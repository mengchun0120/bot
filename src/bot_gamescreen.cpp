#include <algorithm>
#include <cstdio>
#include <cmath>
#include <rapidjson/filereadstream.h>
#include "bot_log.h"
#include "bot_app.h"
#include "bot_utils.h"
#include "bot_inputevent.h"
#include "bot_gamescreen.h"

using namespace rapidjson;

namespace bot {

const float GameScreen::GRID_BREATH = 40.0f;
const int GameScreen::MIN_NUM_ROWS = 20;
const int GameScreen::MAX_NUM_ROWS = 75; 
const int GameScreen::MIN_NUM_COLS = 25;
const int GameScreen::MAX_NUM_COLS = 75;

bool loadMapDimension(int& numRows, int& numCols, const Document& doc)
{
    if (!doc.HasMember("numRows")) {
        LOG_ERROR("Map doesn't specify numRows");
        return false;
    }
    else {
        const Value& v = doc["numRows"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid numRows in map");
            return false;
        }
        numRows = v.GetInt();
        if (numRows < GameScreen::MIN_NUM_ROWS || numRows > GameScreen::MAX_NUM_ROWS) {
            LOG_ERROR("numRows must be in [%d,%d]", GameScreen::MIN_NUM_ROWS, GameScreen::MAX_NUM_ROWS);
            return false;
        }
    }

    if (!doc.HasMember("numCols")) {
        LOG_ERROR("Map doesn't specify numCols");
        return false;
    }
    else {
        const Value& v = doc["numCols"];
        if (!v.IsInt()) {
            LOG_ERROR("Invalid numCols in map");
            return false;
        }
        numCols = v.GetInt();
        if (numCols < GameScreen::MIN_NUM_COLS || numCols > GameScreen::MAX_NUM_COLS) {
            LOG_ERROR("numCols must be in [%d,%d]", GameScreen::MIN_NUM_COLS, GameScreen::MAX_NUM_COLS);
        }
    }

    return true;
}

bool loadMapPlayerSetting(float& startPosX, float& startPosY, float& startDirectionX, float& startDirectionY,
                       const Document& doc)
{
    if (!doc.HasMember("player")) {
        LOG_ERROR("map doesn't have player setting");
        return false;
    }

    const Value& playerSetting = doc["player"];

    if (!playerSetting.HasMember("startPos")) {
        LOG_ERROR("map doesn't have startPos for player");
        return false;
    }

    const Value& startPos = playerSetting["startPos"];
    if (!startPos.IsArray() || startPos.Size() != 2 || !startPos[0].IsFloat() || !startPos[1].IsFloat()) {
        LOG_ERROR("Invalid startPos for player");
        return false;
    }

    startPosX = startPos[0].GetFloat();
    startPosY = startPos[1].GetFloat();

    if (!playerSetting.HasMember("startDirection")) {
        LOG_ERROR("map doesn't have startDirection for player");
        return false;
    }

    const Value& startDirection = playerSetting["startDirection"];
    if (!startDirection.IsArray() || startDirection.Size() != 2 || !startDirection[0].IsFloat()
        || !startDirection[1].IsFloat()) {
        LOG_ERROR("Invalid startDirection for player");
        return false;
    }

    startDirectionX = startDirection[0].GetFloat();
    startDirectionY = startDirection[1].GetFloat();

    return true;
}

GameScreen::GameScreen()
    : m_mapWidth(0.0f)
    , m_mapHeight(0.0f)
    , m_player(nullptr)
    , m_firstObj(nullptr)
    , m_minViewportX(0.0f)
    , m_minViewportY(0.0f)
    , m_maxViewportX(0.0f)
    , m_maxViewportY(0.0f)
    , m_viewportWorldX(0.0f)
    , m_viewportWorldY(0.0f)
{
    m_viewportPos[0] = 0.0f;
    m_viewportPos[1] = 0.0f;
}

GameScreen::~GameScreen()
{
    clearMap();
}

bool GameScreen::init()
{
    std::string mapFile = constructPath({App::g_app.getResourceDir(), "map", "map_01.json"});
    if (!loadMap(mapFile.c_str())) {
        return false;
    }
    return true;
}

bool GameScreen::loadMap(const char* fileName)
{
    LOG_INFO("loadMap: %s", fileName);
    Document doc;
    if (!readJson(doc, fileName)) {
        return false;
    }

    if (!initMap(doc)) {
        return false;
    }

    if (!loadPlayer(doc)) {
        return false;
    }

    if (!loadObjects(doc)) {
        return false;
    }

    updateViewport();
    LOG_INFO("loadMap finished");

    int numRows = getNumRows(), numCols = getNumCols();

    return true;
}

bool GameScreen::initMap(const Document& doc)
{
    int numRows = 0, numCols = 0;

    if (!loadMapDimension(numRows, numCols, doc)) {
        return false;
    }

    m_pool.init(2 * numRows * numCols);

    m_map.resize(numRows);
    for (int i = 0; i < numRows; ++i) {
        m_map[i].resize(numCols);
        for (int j = 0; j < numCols; ++j) {
            m_map[i][j] = nullptr;
        }
    }

    m_mapWidth = numCols * GRID_BREATH;
    m_mapHeight = numRows * GRID_BREATH;

    float windowBreathX = App::g_app.viewportWidth() / 2.0f;
    float windowBreathY = App::g_app.viewportHeight() / 2.0f;

    m_minViewportX = windowBreathX;
    m_maxViewportX = m_mapWidth - windowBreathX;
    m_minViewportY = windowBreathY;
    m_maxViewportY = m_mapHeight - windowBreathY;

    return true;
}

bool GameScreen::loadPlayer(const Document& doc)
{
    LOG_INFO("Loading player");

    float startPosX = 0.0f, startPosY = 0.0f, startDirectionX = 1.0f, startDirectionY = 0.0f;
    if (!loadMapPlayerSetting(startPosX, startPosY, startDirectionX, startDirectionY, doc)) {
        return false;
    }

    const GameLib& lib = App::g_app.gameLib();
    const GameObjectTemplate* playerTemplate = lib.getGameObjectTemplate("player");
    if (!playerTemplate) {
        LOG_ERROR("Failed to find player template");
        return false;
    }

    m_player = GameObject::createFromTemplate(playerTemplate);
    if (!m_player) {
        LOG_ERROR("Failed to create player");
        return false;
    }

    m_player->setPos(startPosX, startPosY);
    m_player->setDirection(startDirectionX, startDirectionY);

    addGameObj(m_player);
    
    LOG_INFO("Finished loading player");

    return true;
}

bool GameScreen::loadObjects(const rapidjson::Document& doc)
{
    using namespace rapidjson;

    LOG_INFO("Loading objects to map");

    const GameLib& lib = App::g_app.gameLib();

    if (!doc.HasMember("objects")) {
        LOG_ERROR("No tiles specified");
        return false;
    }

    const Value& objects = doc["objects"];
    if (!objects.IsArray()) {
        LOG_ERROR("Invalid tiles");
        return false;
    }

    for (SizeType i = 0; i < objects.Size(); ++i) {
        const Value& obj = objects[i];
        if (!obj.IsArray() && obj.Size() < 3 && !obj[0].IsString()) {
            LOG_ERROR("Invalid object %d", i);
            return 0;
        }

        const char* name = obj[0].GetString();
        const GameObjectTemplate* objTemplate = lib.getGameObjectTemplate(name);
        if (!objTemplate) {
            LOG_ERROR("Cannot find GameObjectTemplate %s", name);
            return false;
        }

        if (objTemplate->getType() == GAMEOBJ_BOT) {
            if (obj.Size() != 5) {
                LOG_ERROR("Invalid object %s", name);
                return false;
            }
        }
        else if (obj.Size() != 3) {
            LOG_ERROR("Invalid object %s", name);
            return false;
        }

        for (SizeType i = 1; i < obj.Size(); ++i) {
            if (!obj[i].IsFloat()) {
                LOG_ERROR("Invalid object setting %s", name);
                return false;
            }
        }

        float posX = obj[1].GetFloat();
        float posY = obj[2].GetFloat();

        GameObject* gameObj = GameObject::createFromTemplate(objTemplate);
        gameObj->setPos(posX, posY);

        if (objTemplate->getType() == GAMEOBJ_BOT) {
            float directionX = obj[3].GetFloat();
            float directionY = obj[4].GetFloat();
            gameObj->setDirection(directionX, directionY);
        }
        
        addGameObj(gameObj);
    }

    LOG_INFO("Finished loading objects");
    return true;
}

int GameScreen::update(float delta)
{
    return 0;
}

void GameScreen::present()
{
    int startRow, endRow, startCol, endCol;

    getDisplayRegion(startRow, endRow, startCol, endCol);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GOBJ_FLAG_DRAWN);

    for (int r = startRow; r <= endRow; ++r) {
        std::vector<MapItem*>& row = m_map[r];

        for (int c = startCol; c <= endCol; ++c) {
            for (MapItem* item = row[c]; item; item = static_cast<MapItem*>(item->getNext())) {
                GameObject* obj = item->getObj();
                if (!obj->testFlag(GOBJ_FLAG_DRAWN)) {
                    obj->present();
                    obj->setFlag(GOBJ_FLAG_DRAWN);
                }
            }
        }
    }
}

int GameScreen::processInput(const InputEvent &e)
{
    switch (e.m_type) {
    case InputEvent::ET_MOUSE_MOVE:
        return handleMouseMove(e.m_mouseMoveEvent);
    case InputEvent::ET_MOUSE_BUTTON:
        return handleMouseButton(e.m_mouseButtonEvent);
    case InputEvent::ET_KEY:
        return handleKey(e.m_keyEvent);
    default:
        LOG_WARN("Unknown input type %d", static_cast<int>(e.m_type));
    }
    return 0;
}

int GameScreen::handleMouseMove(const MouseMoveEvent& e)
{
    if (!m_player) {
        return 0;
    }

    float worldX = e.m_x + m_viewportWorldX;
    float worldY = e.m_y + m_viewportWorldY;
    float deltaX = worldX - m_player->getPosX();
    float deltaY = worldY - m_player->getPosY();
    float dist = sqrt(deltaX * deltaX + deltaY * deltaY);
    float directionX = deltaX / dist;
    float directionY = deltaY / dist;

    m_player->setDirection(directionX, directionY);
    LOG_INFO("new direction %f %f", directionX, directionY);

    return 0;
}

int GameScreen::handleMouseButton(const MouseButtonEvent& e)
{
    return 0;
}

int GameScreen::handleKey(const KeyEvent& e)
{
    return 0;
}

bool GameScreen::addGameObj(GameObject* obj)
{
    int startRow, endRow, startCol, endCol;
    bool placeObjInMap = getMapPosForGameObj(startRow, endRow, startCol, endCol, obj);
    if (!placeObjInMap) {
        return false;
    }

    for (int row = startRow; row <= endRow; ++row) {
        for (int col = startCol; col <= endCol; ++col) {
            addGameObjAt(obj, row, col);
        }
    }
 
    obj->setCoverStartRow(startRow);
    obj->setCoverEndRow(endRow);
    obj->setCoverStartCol(startCol);
    obj->setCoverEndCol(endCol);

    obj->setNext(m_firstObj);
    if (m_firstObj) {
        m_firstObj->setPrev(obj);
    }
    m_firstObj = obj;

    return true;
}

bool GameScreen::repositionGameObj(GameObject* obj)
{
    int newStartRow, newEndRow, newStartCol, newEndCol;

    if (!getMapPosForGameObj(newStartRow, newEndRow, newStartCol, newEndCol, obj)) {
        removeGameObj(obj);
        return false;
    }

    int oldStartRow = obj->getCoverStartRow();
    int oldEndRow = obj->getCoverEndRow();
    int oldStartCol = obj->getCoverStartCol();
    int oldEndCol = obj->getCoverEndCol();

    if (newStartRow > oldEndRow || newEndRow < oldStartRow || newStartCol > oldEndCol || newEndCol < oldStartCol) {
        removeGameObjFromRect(obj, oldStartRow, oldEndRow, oldStartCol, oldEndCol);
        addGameObjToRect(obj, newStartRow, newEndRow, newStartCol, newEndCol);
        return true;
    }

    if (newStartRow < oldStartRow) {
        addGameObjToRect(obj, newStartRow, oldStartRow - 1, newStartCol, newEndCol);
    }
    else if (newStartRow > oldStartRow) {
        removeGameObjFromRect(obj, oldStartRow, newStartRow - 1, oldStartCol, oldEndCol);
    }

    if (newEndRow > oldEndRow) {
        addGameObjToRect(obj, oldEndRow + 1, newEndRow, newStartCol, newEndCol);
    }
    else if (newEndRow < oldEndRow) {
        removeGameObjFromRect(obj, newEndRow + 1, oldEndRow, oldStartCol, oldEndCol);
    }

    int overlapStartRow = std::max(newStartRow, oldStartRow);
    int overlapEndRow = std::min(newEndRow, oldEndRow);

    if (newStartCol < oldStartCol) {
        addGameObjToRect(obj, overlapStartRow, overlapEndRow, newStartCol, oldStartCol - 1);
    }
    else if (newStartCol > oldStartCol) {
        removeGameObjFromRect(obj, overlapStartRow, overlapEndRow, oldStartCol, newStartCol - 1);
    }

    if (newEndCol > oldEndCol) {
        addGameObjToRect(obj, overlapStartRow, overlapEndRow, oldEndCol + 1, newEndCol);
    }
    else if (newEndCol < oldEndCol) {
        removeGameObjFromRect(obj, overlapStartRow, overlapEndRow, newEndCol + 1, oldEndCol);
    }

    return true;
}

void GameScreen::removeGameObj(GameObject* obj)
{
    int endRow = obj->getCoverEndRow();
    int endCol = obj->getCoverEndCol();

    for (int row = obj->getCoverStartRow(); row <= endRow; ++row) {
        for (int col = obj->getCoverStartCol(); col <= endCol; ++col) {
            removeGameObjAt(obj, row, col);
        }
    }

    GameObject* prev = static_cast<GameObject*>(obj->getPrev());
    GameObject* next = static_cast<GameObject*>(obj->getNext());
    if (prev) {
        prev->setNext(next);
    }
    else {
        m_firstObj = next;
    }

    if (next) {
        next->setPrev(prev);
    }

    delete obj;
}

bool GameScreen::getMapPosForGameObj(int& startRow, int& endRow, int& startCol, int& endCol, GameObject* obj)
{
    int startRow1 = getMapCoord(obj->getCoverBottom());
    if (startRow1 >= getNumRows()) {
        return false;
    }

    if (startRow1 < 0) {
        startRow1 = 0;
    }

    int endRow1 = getMapCoord(obj->getCoverTop());
    if (endRow1 < 0) {
        return false;
    }

    if (endRow1 >= getNumRows()) {
        endRow1 = getNumRows() - 1;
    }

    int startCol1 = getMapCoord(obj->getCoverLeft());

    if (startCol1 >= getNumCols()) {
        return false;
    }

    if (startCol1 < 0) {
        startCol1 = 0;
    }

    int endCol1 = getMapCoord(obj->getCoverRight());

    if (endCol1 < 0) {
        return false;
    }

    if (endCol1 >= getNumCols()) {
        endCol1 = getNumCols() - 1;
    }

    startRow = startRow1;
    endRow = endRow1;
    startCol = startCol1;
    endCol = endCol1;

    return true;
}

void GameScreen::addGameObjAt(GameObject* obj, int row, int col)
{
    MapItem* newItem = m_pool.alloc();
    newItem->setObj(obj);
    newItem->setNext(m_map[row][col]);
    m_map[row][col] = newItem;
}

MapItem* GameScreen::findGameObjectAt(GameObject* obj, int row, int col)
{
    MapItem* item = m_map[row][col];
    for (; item; item = static_cast<MapItem*>(item->getNext())) {
        if (item->getObj() == obj) {
            break;
        }
    }

    return item;
}

void GameScreen::removeGameObjAt(GameObject* obj, int row, int col)
{
    MapItem* prev = nullptr;
    MapItem* item = m_map[row][col];

    for (; item; item = static_cast<MapItem*>(item->getNext())) {
        if (item->getObj() == obj) {
            break;
        }
        prev = item;
    }

    if (!item) {
        return;
    }

    if (prev) {
        prev->setNext(item->getNext());
    }
    else {
        m_map[row][col] = static_cast<MapItem*>(item->getNext());
    }

    m_pool.free(item);
}

void GameScreen::removeGameObjFromRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            removeGameObjAt(obj, r, c);
        }
    }
}

void GameScreen::addGameObjToRect(GameObject* obj, int startRow, int endRow, int startCol, int endCol)
{
    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            addGameObjAt(obj, r, c);
        }
    }
}

void GameScreen::clearMap()
{
    int numRows = getNumRows();
    int numCols = getNumCols();

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c) {
            MapItem* item, * next;
            for (item = m_map[r][c]; item; item = next) {
                next = static_cast<MapItem*>(item->getNext());
                m_pool.free(item);
            }
        }
    }

    GameObject* curObj, * nextObj;
    for (curObj = m_firstObj; curObj; curObj = nextObj) {
        nextObj = static_cast<GameObject*>(curObj->getNext());
        delete curObj;
    }
}

void GameScreen::updateViewport()
{
    m_viewportPos[0] = clamp(m_player->getPosX(), m_minViewportX, m_maxViewportX);
    m_viewportPos[1] = clamp(m_player->getPosY(), m_minViewportY, m_maxViewportY);
    m_viewportWorldX = m_viewportPos[0] - App::g_app.viewportWidth() / 2.0f;
    m_viewportWorldY = m_viewportPos[1] - App::g_app.viewportHeight() / 2.0f;
    App::g_app.program().setViewportOrigin(m_viewportPos);
}

void GameScreen::getDisplayRegion(int& startRow, int& endRow, int& startCol, int& endCol)
{
    float windowBreathX = App::g_app.viewportWidth() / 2.0f;
    float windowBreathY = App::g_app.viewportWidth() / 2.0f;

    startCol = getMapCoord(m_viewportPos[0] - windowBreathX);
    if (startCol < 0) {
        startCol = 0;
    }

    endCol = getMapCoord(m_viewportPos[0] + windowBreathX);
    if (endCol >= getNumCols()) {
        endCol = getNumCols() - 1;
    }
    
    
    startRow = getMapCoord(m_viewportPos[1] - windowBreathY);
    if (startRow < 0) {
        startRow = 0;
    }

    endRow = getMapCoord(m_viewportPos[1] + windowBreathY);
    if (endRow >= getNumRows()) {
        endRow = getNumRows() - 1;
    }
}

void GameScreen::clearFlagsInRect(int startRow, int endRow, int startCol, int endCol, GameObjectFlag flag)
{
    for (int r = startRow; r <= endRow; ++r) {
        std::vector<MapItem*>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) {
            for (MapItem* item = m_map[r][c]; item; item = static_cast<MapItem*>(item->getNext())) {
                item->getObj()->clearFlag(flag);
            }
        }
    }
}

} // end of namespace bot
