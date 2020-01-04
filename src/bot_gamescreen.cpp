#include <algorithm>
#include <rapidjson/filereadstream.h>
#include "bot_log.h"
#include "bot_app.h"
#include "bot_inputevent.h"
#include "bot_gamescreen.h"

namespace bot {

const float GameScreen::GRID_BREATH = 40.0f;
const int GameScreen::MIN_NUM_ROWS = 20;
const int GameScreen::MAX_NUM_ROWS = 75; 
const int GameScreen::MIN_NUM_COLS = 25;
const int GameScreen::MAX_NUM_COLS = 75;

bool readMapDimension(int& numRows, int& numCols, const rapidjson::Document& doc)
{

}

GameScreen::GameScreen()
    : m_mapWidth(0.0f)
    , m_mapHeight(0.0f)
{
}

GameScreen::~GameScreen()
{
    clearMap();
}

bool GameScreen::init()
{
    return true;
}

bool GameScreen::loadMap(const char* fileName)
{
    using namespace rapidjson;

    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        LOG_ERROR("Failed to open %s", fileName);
        return false;
    }

    char readBuffer[1000];
    FileReadStream stream(fp, readBuffer, sizeof(readBuffer));
    Document doc;

    doc.ParseStream(stream);
    fclose(fp);

    if (doc.HasParseError()) {
        LOG_ERROR("Failed to parse %s", fileName);
        return false;
    }

    if (!initMap(doc)) {
        return false;
    }


}

bool GameScreen::initMap(const rapidjson::Document& doc)
{
    using namespace rapidjson;

    int numRows, numCols;

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

    return true;
}

bool GameScreen::loadTiles(const rapidjson::Document& doc)
{
    using namespace rapidjson;

    if (!doc.HasMember("tiles")) {
        LOG_ERROR("No tiles specified");
        return false;
    }

    const Value& v = doc["tiles"];
    if (!v.IsArray()) {
        LOG_ERROR("Invalid tiles");
        return false;
    }

    for (SizeType i = 0; i < v.Size(); ++i) {

    }

    return true;
}

int GameScreen::update(float delta)
{
    return 0;
}

void GameScreen::present()
{
}

int GameScreen::processInput(const InputEvent &e)
{
    return 0;
}

bool GameScreen::addGameObj(GameObject* obj)
{
    int startRow, endRow, startCol, endCol;
    if (!getMapPosForGameObj(startRow, endRow, startCol, endCol, obj)) {
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
}

} // end of namespace bot
