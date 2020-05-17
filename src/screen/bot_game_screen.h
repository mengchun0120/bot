#ifndef INCLUDE_BOT_GAME_SCREEN
#define INCLUDE_BOT_GAME_SCREEN

#include "screen/bot_screen.h"
#include "gameutil/bot_game_map.h"
#include "gameutil/bot_game_object_manager.h"
#include "gameutil/bot_game_state.h"

namespace bot {

struct MouseMoveEvent;
struct MouseButtonEvent;
struct KeyEvent;
class App;

class GameScreen: public Screen {
public:
    GameScreen(App* app);

    virtual ~GameScreen();

    virtual bool init();

    bool loadMap(const std::string& fileName);

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

    GameMap& getMap()
    {
        return m_map;
    }

    const GameMap& getMap() const
    {
        return m_map;
    }

    GameObjectManager& getGameObjManager()
    {
        return m_gameObjManager;
    }

    const GameObjectManager& getGameObjManager() const
    {
        return m_gameObjManager;
    }

private:
    bool updateRobots(float delta);

    bool updateMissiles(float delta);

    void updateEffects(float delta);

    void presentEffects();

    void clearDeadObjects();

    int handleMouseMove(const MouseMoveEvent& e);

    int handleMouseButton(const MouseButtonEvent& e);

    int handleKey(const KeyEvent& e);

private:
    App* m_app;
    GameMap m_map;
    GameObjectManager m_gameObjManager;
    GameState m_state;
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

