#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_math_utils.h"
#include "input/bot_input_event.h"
#include "gameobj/bot_game_object_type.h"
#include "gameobj/bot_tile.h"
#include "gameobj/bot_missile.h"
#include "gameobj/bot_player.h"
#include "gameutil/bot_game_map_loader.h"
#include "app/bot_app.h"
#include "screen/bot_game_screen.h"

using namespace rapidjson;

namespace bot {

GameScreen::GameScreen(App* app)
    : m_app(app)
    , m_gameObjManager(app->getGameTemplateLib(), app->getMissilePoolSize())
    , m_state(GameState::INIT)
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

    m_state = GameState::RUNNING;

    return true;
}

bool GameScreen::loadMap(const std::string& fileName)
{
    LOG_INFO("Loading map from %s", fileName.c_str());

    GameMapLoader mapLoader(m_map, m_gameObjManager, m_app->getMapPoolFactor());
    if (!mapLoader.load(fileName, m_app->getViewportWidth(), m_app->getViewportHeight()))
    {
        return false;
    }

    float windowBreathX = m_app->getViewportWidth() / 2.0f;
    float windowBreathY = m_app->getViewportHeight() / 2.0f;

    LOG_INFO("Done loading map");

    return true;
}

int GameScreen::update(float delta)
{
    if (m_state != GameState::RUNNING)
    {
        return 0;
    }

    Player* player = m_map.getPlayer();
    if (player)
    {
        if (!player->update(delta, *this)) {
            LOG_INFO("player is DEAD");
        }
    }

    updateViewport();
    updateRobots(delta);
    updateMissiles(delta);
    clearDeadObjects();

    return 0;
}

void GameScreen::present()
{
    static const std::vector<GameObjectType> LAYER_ORDER{
        GameObjectType::TILE, GameObjectType::MISSILE, GameObjectType::ROBOT, GameObjectType::ANIMATION
    };
    static const int NUM_LAYERS = static_cast<int>(LAYER_ORDER.size());

    int startRow, endRow, startCol, endCol;

    m_map.getViewportRegion(startRow, endRow, startCol, endCol);
    m_map.clearFlagsInRect(startRow, endRow, startCol, endCol, GameObjectFlag::DRAWN);

    for (int i = 0; i < NUM_LAYERS; ++i)
    {
        for (int r = startRow; r <= endRow; ++r)
        {
            for (int c = startCol; c <= endCol; ++c)
            {
                LinkedList<GameObjectItem>& cell = m_map.getMapCell(r, c);
                for (GameObjectItem* item = cell.getFirst(); item; item = static_cast<GameObjectItem*>(item->getNext()))
                {
                    GameObject* obj = item->getObj();
                    if (obj->getType() != LAYER_ORDER[i])
                    {
                        continue;
                    }

                    if (obj->testFlag(static_cast<int>(GameObjectFlag::DRAWN)))
                    {
                        continue;
                    }

                    obj->present(m_app->getSimpleShaderProgram());
                    obj->setFlag(static_cast<int>(GameObjectFlag::DRAWN));
                }
            }
        }
    }
}

int GameScreen::processInput(const InputEvent& e)
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


void GameScreen::updateViewport()
{
    m_map.updateViewport();
    m_app->getSimpleShaderProgram().setViewportOrigin(m_map.getViewportPos());
}

bool GameScreen::updateRobots(float delta)
{
    const int DONT_UPDATE_FLAG = static_cast<int>(GameObjectFlag::UPDATED) | static_cast<int>(GameObjectFlag::DEAD);
    int startRow, endRow, startCol, endCol;

    m_map.getViewportRegion(startRow, endRow, startCol, endCol);
    m_map.clearFlagsInRect(startRow, endRow, startCol, endCol, GameObjectFlag::UPDATED);

    for (int r = startRow; r <= endRow; ++r)
    {
        for (int c = startCol; c <= endCol; ++c)
        {
            LinkedList<GameObjectItem>& cell = m_map.getMapCell(r, c);
            GameObjectItem* next = nullptr;
            for (GameObjectItem* item = cell.getFirst(); item; item = next)
            {
                next = static_cast<GameObjectItem*>(item->getNext());
                GameObject* obj = item->getObj();

                bool dontUpdate = obj->getType() != GameObjectType::ROBOT ||
                                  obj->testFlag(DONT_UPDATE_FLAG) ||
                                  obj == static_cast<GameObject*>(m_map.getPlayer());
                if (dontUpdate)
                {
                    continue;
                }

                Robot* robot = static_cast<Robot*>(obj);
                if (!robot->update(delta, *this))
                {
                    robot->setFlag(GameDEAD);
                    m_gameObjManager.sendObjectToDeath(robot);
                }

                robot->setFlag(GAME_OBJ_FLAG_UPDATED);
            }
        }
    }

    return true;
}

bool GameScreen::updateMissiles(float delta)
{
    Missile* next = nullptr;
    for (Missile* missile = m_gameObjManager.getFirstActiveMissile(); missile; missile = next)
    {
        next = static_cast<Missile*>(missile->getNext());

        if (!missile->update(delta, *this))
        {
            missile->setFlag(GAME_OBJ_FLAG_DEAD);
            m_gameObjManager.sendObjectToDeath(missile);
        }
    }

    return true;
}

int GameScreen::handleMouseMove(const MouseMoveEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD)) 
    {
        return 0;
    }

    float destX = m_map.getWorldX(e.m_x);
    float destY = m_map.getWorldY(e.m_y);
    float directionX, directionY;
    calculateDirection(directionX, directionY, player->getPosX(), player->getPosY(), destX, destY);
    player->setDirection(directionX, directionY);
    return 0;
}

int GameScreen::handleMouseButton(const MouseButtonEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD))
    {
        return 0;
    }

    if (e.m_button == GLFW_MOUSE_BUTTON_LEFT) 
    {
        player->setShootingEnabled(e.m_action == GLFW_PRESS);
    }

    return 0;
}

int GameScreen::handleKey(const KeyEvent& e)
{
    Player* player = m_map.getPlayer();
    if (!player || player->testFlag(GAME_OBJ_FLAG_DEAD)) {
        return 0;
    }

    switch (e.m_key) 
    {
        /*case GLFW_KEY_F:
            return handleFireKey(e.m_action);*/
        case GLFW_KEY_W:
            player->setMovingEnabled(true);
            break;
        case GLFW_KEY_S:
            player->setMovingEnabled(false);
            break;
    }
    
    return 0;
}

void GameScreen::clearDeadObjects()
{
    GameObject* next = nullptr;
    for (GameObject* obj = m_gameObjManager.getFirstDeadObject(); obj; obj = next)
    {
        next = static_cast<GameObject*>(obj->getNext());
        m_map.removeObject(obj);
    }

    m_gameObjManager.clearDeadObjects();
}

} // end of namespace bot
