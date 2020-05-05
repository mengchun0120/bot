#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include "screen/bot_screen.h"
#include "gameutil/bot_gamemap.h"
#include "gameutil/bot_gameobjectmanager.h"

namespace bot {

class MouseMoveEvent;
class MouseButtonEvent;
class KeyEvent;
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
    void updateViewport();

    bool updateRobots(float delta);

    bool updateMissiles(float delta);

    void clearDeadObjects();

    int handleMouseMove(const MouseMoveEvent& e);

    int handleMouseButton(const MouseButtonEvent& e);

    int handleKey(const KeyEvent& e);

private:
    App* m_app;
    GameMap m_map;
    GameObjectManager m_gameObjManager;
};

} // end of namespace bot

#endif

