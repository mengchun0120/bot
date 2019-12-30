#ifndef INCLUDE_BOT_GAMESCREEN
#define INCLUDE_BOT_GAMESCREEN

#include "bot_gameobject.h"
#include "bot_screen.h"

namespace bot {

class GameScreen: public Screen {
public:
    GameScreen();

    virtual ~GameScreen();

    virtual bool init();

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

public:
    GameObject m_obj;
};

} // end of namespace bot

#endif

