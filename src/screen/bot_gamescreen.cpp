#include "misc/bot_log.h"
#include "misc/bot_fileutils.h"
#include "app/bot_app.h"
#include "input/bot_inputevent.h"
#include "gameobj/bot_gameobjecttype.h"
#include "gameobj/bot_tile.h"
#include "gameutil/bot_gamemaploader.h"
#include "screen/bot_gamescreen.h"

using namespace rapidjson;

namespace bot {

GameScreen::GameScreen(App* app)
    : m_app(app)
    , m_gameObjManager(app->getGameTemplateLib())
{
}

GameScreen::~GameScreen()
{
}

bool GameScreen::init()
{
    LOG_INFO("Initializing GameScreen");

    if (!loadMap(m_app->getMapFile()))
    {
        LOG_ERROR("Failed to load map from %s", m_app->getMapFile().c_str());
        return false;
    }

    updateViewport();

    return true;
}

bool GameScreen::loadMap(const std::string& fileName)
{
    LOG_INFO("Loading map from %s", fileName.c_str());

    GameMapLoader mapLoader(m_map, m_gameObjManager, m_app->getMapPoolFactor());
    if (!mapLoader.load(fileName))
    {
        return false;
    }

    float windowBreathX = m_app->getViewportWidth() / 2.0f;
    float windowBreathY = m_app->getViewportHeight() / 2.0f;

    m_minViewportX = windowBreathX;
    m_maxViewportX = m_map.getMapWidth() - windowBreathX;
    m_minViewportY = windowBreathY;
    m_maxViewportY = m_map.getMapHeight() - windowBreathY;

    LOG_INFO("%f %f", m_minViewportX, m_minViewportY);

    LOG_INFO("Done loading map");

    return true;
}

int GameScreen::update(float delta)
{
    return 0;
}

void GameScreen::present()
{
    static const std::vector<GameObjectType> LAYER_ORDER{GAME_OBJ_TYPE_TILE, GAME_OBJ_TYPE_MISSILE, GAME_OBJ_TYPE_BOT};
    static const int NUM_LAYERS = static_cast<int>(LAYER_ORDER.size());

    int startRow, endRow, startCol, endCol;
    float windowBreathX = m_app->getViewportWidth() / 2.0f;
    float windowBreathY = m_app->getViewportHeight() / 2.0f;
    float left = m_viewportPos[0] - windowBreathX;
    float bottom = m_viewportPos[1] - windowBreathY;
    float right = m_viewportPos[0] + windowBreathX;
    float top = m_viewportPos[1] + windowBreathY;

    m_map.getRectCoords(startRow, endRow, startCol, endCol, left, bottom, right, top);
    m_map.clearFlagsInRect(startRow, endRow, startCol, endCol, GAME_OBJ_FLAG_DRAWN);

    for (int i = 0; i < NUM_LAYERS; ++i)
    {
        for (int r = startRow; r <= endRow; ++r)
        {
            for (int c = startCol; c <= endCol; ++c)
            {
                GameMap::MapCell& cell = m_map.getMapCell(r, c);
                for (MapItem* item = cell.getFirst(); item; item = static_cast<MapItem*>(item->getNext()))
                {
                    GameObject* obj = item->getObj();
                    if (obj->getType() != LAYER_ORDER[i])
                    {
                        continue;
                    }

                    if (obj->testFlag(GAME_OBJ_FLAG_DRAWN))
                    {
                        continue;
                    }

                    obj->present(m_app->getSimpleShaderProgram());
                    obj->setFlag(GAME_OBJ_FLAG_DRAWN);
                }
            }
        }
    }
}

int GameScreen::processInput(const InputEvent& e)
{
    return 0;
}


void GameScreen::updateViewport()
{
    m_viewportPos[0] = m_minViewportX;
    m_viewportPos[1] = m_minViewportY;
    m_app->getSimpleShaderProgram().setViewportOrigin(m_viewportPos);
    //m_viewportPos[0] = clamp(m_player->getPosX(), m_minViewportX, m_maxViewportX);
    //m_viewportPos[1] = clamp(m_player->getPosY(), m_minViewportY, m_maxViewportY);
    //m_viewportWorldX = m_viewportPos[0] - App::g_app.viewportWidth() / 2.0f;
    //m_viewportWorldY = m_viewportPos[1] - App::g_app.viewportHeight() / 2.0f;
    //App::g_app.program().setViewportOrigin(m_viewportPos);
}

/*

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

    m_player = new GameObject(playerTemplate);

    m_player->setPos(startPosX, startPosY);
    m_player->setDirection(startDirectionX, startDirectionY);

    addGameObj(m_player);
    
    LOG_INFO("Finished loading player");

    return true;
}


int GameScreen::update(float delta)
{
    if (m_player) {
        int rc = updatePlayer(delta);
        if (0 != rc) {
            return rc;
        }
    }

    m_player->setFlag(GOBJ_FLAG_UPDATED);

    int startRow, endRow, startCol, endCol;

    getDisplayRegion(startRow, endRow, startCol, endCol);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GOBJ_FLAG_UPDATED);

    for (int r = startRow; r <= endRow; ++r) {
        std::vector<MapItem*>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) {
            for (MapItem* item = row[c]; item; item = static_cast<MapItem*>(item->getNext())) {
                GameObject* obj = item->getObj();

                if (obj->getType() == GAMEOBJ_TILE || obj->testFlag(GOBJ_FLAG_UPDATED) || 
                    obj->testFlag(GOBJ_FLAG_DEAD)) {
                    continue;
                }

                updateObject(obj, delta);
                obj->setFlag(GOBJ_FLAG_UPDATED);
            }
        }
    }

    return 0;
}

void GameScreen::present()
{
    static const std::vector<GameObjectType> PRESENT_ORDER{GAMEOBJ_TILE, GAMEOBJ_BULLET, GAMEOBJ_BOT};
    int startRow, endRow, startCol, endCol;

    getDisplayRegion(startRow, endRow, startCol, endCol);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GOBJ_FLAG_DRAWN);

    for (auto objTypeIt = PRESENT_ORDER.begin(); objTypeIt != PRESENT_ORDER.end(); objTypeIt++) {
        for (int r = startRow; r <= endRow; ++r) {
            std::vector<MapItem*>& row = m_map[r];
            for (int c = startCol; c <= endCol; ++c) {
                for (MapItem* item = row[c]; item; item = static_cast<MapItem*>(item->getNext())) {
                    GameObject* obj = item->getObj();
                    if (obj->getType() != *objTypeIt || obj->testFlag(GOBJ_FLAG_DRAWN)) {
                        continue;
                    }
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

    float destX = getWorldX(e.m_x);
    float destY = getWorldY(e.m_y);
    m_player->setDirectionByDest(destX, destY);

    return 0;
}

int GameScreen::handleMouseButton(const MouseButtonEvent& e)
{
    if (!m_player) {
        return 0;
    }
    
    if (e.m_button == GLFW_MOUSE_BUTTON_LEFT && e.m_action == GLFW_PRESS) {
        float destX = getWorldX(e.m_x);
        float destY = getWorldY(e.m_y);
        m_player->setDirectionByDest(destX, destY);
        m_player->setDest(destX, destY);
        m_player->setMovability(true);
    }

    return 0;
}

int GameScreen::handleKey(const KeyEvent& e)
{
    if (!m_player) {
        return 0;
    }

    switch (e.m_key) {
    case GLFW_KEY_F:
        return handleFireKey(e.m_action);
    }

    return 0;
}

int GameScreen::handleFireKey(int action) 
{
    if (action == GLFW_PRESS) {
        m_player->startFiring();
    }
    else if (action == GLFW_RELEASE) {
        m_player->endFiring();
    }

    return 0;
}

void GameScreen::updateObject(GameObject* obj, float delta)
{
    
}

bool GameScreen::checkOutsideMap(float x, float y)
{
    return x <= 0.0f || x >= m_mapWidth || y <= 0.0f || y >= m_mapHeight;
}

bool GameScreen::testCollision(float collideLeft, float collideBottom, float collideRight, float collideTop, 
                                int excludeSide, GameObjectType excludeType)
{
    int startRow = getRow(collideBottom), endRow = getRow(collideTop);
    int startCol = getCol(collideLeft), endCol = getCol(collideRight);

    clearFlagsInRect(startRow, endRow, startCol, endCol, GOBJ_FLAG_CHECKED);

    for (int r = startRow; r <= endRow; ++r) {
        std::vector<MapItem*>& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) {
            for (MapItem* item = row[c]; item; item = static_cast<MapItem*>(item->getNext())) {
                GameObject* obj = item->getObj();
                if (obj->testFlag(GOBJ_FLAG_CHECKED)) {
                    continue;
                }

                if (obj->getSide() != excludeSide && obj->getType() != excludeType) {
                    bool overlap = checkRectOverlapp(collideLeft, collideBottom, collideRight, collideTop, 
                                                     obj->getCollideLeft(), obj->getCollideBottom(), 
                                                     obj->getCollideRight(), obj->getCollideTop());
                    if (overlap) {
                        return true;
                    }
                }

                obj->setFlag(GOBJ_FLAG_CHECKED);
            }
        }
    }

    return false;
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

int GameScreen::updatePlayer(float delta)
{
    if (m_player->isMoving()) {
        int rc = movePlayerToDest(delta);
        if (0 != rc) {
            return rc;
        }
    }

    return 0;
}

int GameScreen::movePlayerToDest(float delta)
{
    float speed = m_player->getSpeed();
    float speedX = speed * m_player->getDirectionX();
    float speedY = speed * m_player->getDirectionY();
    float finalDelta;
    bool stopMoving = false;

    if (checkMoveWithinBoundary(finalDelta, m_player, speedX, speedY, delta)) {
        stopMoving = true;
    }

    if (checkMoveThroughObjects(finalDelta, m_player, speedX, speedY, finalDelta)) {
        stopMoving = true;
    }

    if (!stopMoving && checkMoveToDest(finalDelta, m_player, finalDelta)) {
        stopMoving = true;
    }

    if (stopMoving) {
        m_player->setMovability(false);
    }

    float deltaX = speedX * finalDelta;
    float deltaY = speedY * finalDelta;
    m_player->move(deltaX, deltaY);
    LOG_INFO("after x=%f y=%f", m_player->getPosX(), m_player->getPosY());
    repositionGameObj(m_player);

    updateViewport();

    return 0;
}

bool GameScreen::checkMoveWithinBoundary(float& newDelta, GameObject* obj, float speedX, float speedY, float delta)
{
    bool touchBoundary = false;

    newDelta = delta;

    float absSpeedX = abs(speedX);
    if (absSpeedX > ZERO) {
        float dist = absSpeedX * delta;
        float maxDist = speedX > 0.0f ? m_mapWidth - obj->getCollideRight() : obj->getCollideLeft();
        if (maxDist < 0.0f) {
            maxDist = 0.0f;
        }

        if (dist > maxDist) {
            touchBoundary = true;
            newDelta = maxDist / absSpeedX;
        }
    }

    float absSpeedY = abs(speedY);
    if (absSpeedY > ZERO) {
        float dist = absSpeedY * delta;
        float maxDist = speedY > 0.0f ? m_mapHeight - obj->getCollideTop() : obj->getCollideBottom();
        if (maxDist < 0.0f) {
            maxDist = 0.0f;
        }

        if (dist > maxDist) {
            touchBoundary = true;
            float d = maxDist / absSpeedY;
            if (d < newDelta) {
                newDelta = d;
            }
        }
    }

    return touchBoundary;
}

bool GameScreen::checkMoveThroughObjects(float& newDelta, GameObject* obj, float speedX, float speedY, 
                                         int excludeSide, GameObjectType excludeType, float delta)
{
    int startRow, endRow, startCol, endCol;
    
    getCollisionCheckRegion(startRow, endRow, startCol, endCol, obj, speedX, speedY, delta);
    clearFlagsInRect(startRow, endRow, startCol, endCol, GOBJ_FLAG_CHECKED);

    bool collide = false;
    float x1 = obj->getPosX();
    float y1 = obj->getPosY();
    float collideBreathX1 = obj->getCollideBreathX();
    float collideBreathY1 = obj->getCollideBreathY();
    
    newDelta = delta;
    obj->setFlag(GOBJ_FLAG_CHECKED);
    for (int r = startRow; r <= endRow; ++r) {
        auto& row = m_map[r];
        for (int c = startCol; c <= endCol; ++c) {
            for (MapItem* item = row[c]; item; item = static_cast<MapItem*>(item->getNext())) {
                GameObject* obj2 = item->getObj();
                if (obj2->testFlag(GOBJ_FLAG_CHECKED)) {
                    continue;
                }
                
                if (obj2->getSide() != excludeSide && obj2->getType() != excludeType) {
                    float tmpDelta;

                    bool tmpCollide = checkObjCollision(tmpDelta, x1, y1, collideBreathX1, collideBreathY1, 
                                                        speedX, speedY, obj2->getPosX(), obj2->getPosY(), 
                                                        obj2->getCollideBreathX(), obj2->getCollideBreathY(), newDelta);

                    if (tmpCollide) {
                        newDelta = tmpDelta;
                        collide = true;
                    }
                }

                obj2->setFlag(GOBJ_FLAG_CHECKED);
            }
        }
    }

    return collide;
}

bool GameScreen::checkMoveToDest(float& newDelta, GameObject* obj, float delta)
{
    bool reach = false;
    MoveAbility* moveAbility = static_cast<MoveAbility*>(obj->getBase().getAbility(ABILITY_MOVE));

    newDelta = delta;
    
    float dist = moveAbility->getSpeed() * newDelta;    
    float maxDeltaX = moveAbility->getDestX() - obj->getPosX();
    float maxDeltaY = moveAbility->getDestY() - obj->getPosY();
    float maxDist = sqrt(maxDeltaX * maxDeltaX + maxDeltaY * maxDeltaY);

    if (dist > maxDist) {
        reach = true;
        newDelta = maxDist / moveAbility->getSpeed();
    }

    return reach;
}

void GameScreen::getCollisionCheckRegion(int& startRow, int& endRow, int& startCol, int& endCol, const GameObject* obj,
                                         float speedX, float speedY, float delta)
{
    float left = obj->getCollideLeft();
    float right = obj->getCollideRight();
    if (speedX < 0.0f) {
        left += speedX * delta;
    }
    else if (speedX > 0.0f) {
        right += speedX * delta;
    }

    float bottom = obj->getCollideBottom();
    float top = obj->getCollideTop();
    if (speedY < 0.0f) {
        bottom += speedY * delta;
    }
    else if (speedY > 0.0f) {
        top += speedY * delta;
    }

    startRow = getMapCoord(bottom);
    startRow = clamp(startRow, 0, getNumRows() - 1);
    endRow = getMapCoord(top);
    endRow = clamp(endRow, 0, getNumRows() - 1);
    startCol = getMapCoord(left);
    startCol = clamp(startCol, 0, getNumCols() - 1);
    endCol = getMapCoord(right);
    endCol = clamp(endCol, 0, getNumCols() - 1);
}
*/
} // end of namespace bot
