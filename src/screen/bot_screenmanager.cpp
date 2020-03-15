#include "screen/bot_startscreen.h"
#include "screen/bot_gamescreen.h"
#include "screen/bot_screenmanager.h"

namespace bot {

ScreenManager::ScreenManager()
{
    m_prevScreen = nullptr;
    m_curScreenType = SCREEN_START;
    m_curScreen = new StartScreen();
}

ScreenManager::~ScreenManager()
{
    if(m_prevScreen) {
        delete m_prevScreen;

    }

    if(m_curScreen) {
        delete m_curScreen;
    }
}

void ScreenManager::init()
{
    m_curScreen->init();
}

int ScreenManager::update(float delta)
{
    int ret = m_curScreen->update(delta);
    if(ret == 1) {
        // switched to another screen
        delete m_prevScreen;
        m_prevScreen = nullptr;
    }
    return ret;
}

void ScreenManager::present()
{
    m_curScreen->present();
}

int ScreenManager::processInput(const InputEvent& e)
{
    int ret = m_curScreen->processInput(e);
    if(ret == 1) {
        // switched to another screen
        delete m_prevScreen;
        m_prevScreen = nullptr;
    }
    return ret;
}

void ScreenManager::switchScreen(ScreenType type)
{
    if(m_curScreenType == type) {
        return;
    }

    Screen *screen = nullptr;
    switch(type) {
    case SCREEN_START:
        screen = new StartScreen();
        break;
    case SCREEN_GAME:
        screen = new GameScreen();
        break;
    }

    m_prevScreen = m_curScreen;
    m_curScreen = screen;
    m_curScreen->init();
}

} // end of namespace bot

