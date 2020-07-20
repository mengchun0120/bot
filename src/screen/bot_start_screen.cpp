#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "input/bot_input_event.h"
#include "widget/bot_button.h"
#include "screen/bot_start_screen.h"
#include "app/bot_app.h"

namespace bot {

StartScreen::StartScreen()
{
    const App& app = App::getInstance();
    m_viewportOrigin[0] = app.getViewportWidth() / 2.0f;
    m_viewportOrigin[1] = app.getViewportHeight() / 2.0f;
}

StartScreen::~StartScreen()
{
}

bool StartScreen::init()
{
    const App& app = App::getInstance();
    const StartScreenConfig& cfg = app.getGameLib().getStartScreenConfig();
    const Rectangle* rect = cfg.getButtonRect();
    float spacing = cfg.getButtonSpacing();
    const std::vector<std::string>& buttonTexts = cfg.getButtonTexts();

    int n = static_cast<int>(buttonTexts.size());
    float x = (app.getViewportWidth() - rect->width()) / 2.0f;
    float y = (app.getViewportHeight() + n * rect->height() + (n - 1) * spacing) / 2.0f;
    float deltaY = rect->height() + spacing;
    
    std::vector<Button::ActionFunc> funcs = {
        std::bind(&StartScreen::startGame, this),
        std::bind(&StartScreen::loadGame, this),
        std::bind(&StartScreen::showSettings, this),
        std::bind(&StartScreen::exitApp, this)
    };

    m_buttons.init(n);
    for (int i = 0; i < n; ++i, y -= deltaY) {
        Button* button = new Button();
        if (!button->init(rect, buttonTexts[i]))
        {
            LOG_ERROR("Failed to initialize start game button");
            return false;
        }
        button->setPos(x, y);
        button->setActionFunc(funcs[i]);
        m_buttons.setWidget(i, button);
    }
    
    return true;
}

int StartScreen::update(float delta)
{
    return 0;
}

void StartScreen::present()
{
    App& app = App::getInstance();
    SimpleShaderProgram& program = app.getSimpleShaderProgram();

    program.use();
    program.setViewportSize(app.getViewportSize());
    program.setViewportOrigin(m_viewportOrigin);

    m_buttons.present();
}

int StartScreen::processInput(const InputEvent& e)
{
    return m_buttons.processInput(e);
}

int StartScreen::startGame()
{
    ScreenManager& screenMgr = App::getInstance().getScreenManager();
    screenMgr.switchScreen(ScreenManager::SCREEN_GAME);
    return 1;
}

int StartScreen::loadGame()
{
    return 0;
}

int StartScreen::showSettings()
{
    return 0;
}

int StartScreen::exitApp()
{
    return 2;
}

} // end of namespace bot
