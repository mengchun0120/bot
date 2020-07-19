#include "misc/bot_log.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

ScreenManager::ScreenManager()
    : m_curScreenType(SCREEN_NONE)
    , m_prevScreen(nullptr)
    , m_curScreen(nullptr)
{
}

ScreenManager::~ScreenManager()
{
    if (m_prevScreen) 
    {
        delete m_prevScreen;

    }

    if (m_curScreen) 
    {
        delete m_curScreen;
    }
}

void ScreenManager::init()
{
    m_curScreen = new StartScreen();
    m_curScreen->init();
    m_curScreenType = SCREEN_START;
}

int ScreenManager::update(float delta)
{
    int ret = m_curScreen->update(delta);
    if (ret == 1) 
    {
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
    if (ret == 1) 
    {
        // switched to another screen
        delete m_prevScreen;
        m_prevScreen = nullptr;
    }
    return ret;
}

void ScreenManager::switchScreen(ScreenType type)
{
    LOG_INFO("Switching screen from %d to %d", static_cast<int>(m_curScreenType), static_cast<int>(type));

    if (m_curScreenType == type) 
    {
        return;
    }

    Screen *screen = nullptr;
    switch (type) 
    {
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

    LOG_INFO("Done switching screen");
}

} // end of namespace bot

