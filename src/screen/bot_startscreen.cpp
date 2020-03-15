#include "misc/bot_log.h"
#include "misc/bot_config.h"
#include "misc/bot_utils.h"
#include "app/bot_app.h"
#include "input/bot_inputevent.h"
#include "screen/bot_startscreen.h"

namespace bot {

StartScreen::StartScreen()
{
    m_texts[0] = "Start Game";
    m_texts[1] = "Load Game";
    m_texts[2] = "Settings";
    m_texts[3] = "Exit";
    fillColor(m_normalTextColor, 0, 0, 135, 255);
    fillColor(m_hoverTextColor, 0, 255, 0, 255);
    fillColor(m_pressedTextColor, 255, 128, 0, 255);
    m_hoverButtonIdx = -1;
    m_pressedButtonIdx = -1;
}

StartScreen::~StartScreen()
{
}

bool StartScreen::init()
{
    std::string buttonFile = constructPath({App::g_app.getResourceDir(), "ui", "blue_button.png"});
    if(!m_button.load(buttonFile)) {
        return false;
    }

    if(!m_rect.init(320, 60, true)) {
        return false;
    }

    setViewportOrigin();
    getButtonPos();
    getTextPos();

    return true;
}

void StartScreen::setViewportOrigin()
{
    App &app = App::g_app;
    float viewportOrigin[] = {app.viewportWidth() / 2.0f, app.viewportHeight() / 2.0f};
    app.program().setViewportOrigin(viewportOrigin);
}

void StartScreen::getButtonPos()
{
    float x = App::g_app.viewportWidth() / 2.0f;
    const float SPACING = 20.0f;
    float y = (App::g_app.viewportHeight() +
               static_cast<float>(m_rect.height()) * (NUM_BUTTONS + 1) +
               SPACING * (NUM_BUTTONS - 1)) / 2.0f;
    float incr = SPACING + m_rect.height();

    for(int i = 0; i < NUM_BUTTONS; ++i) {
        m_buttonPos[i][0] = x;
        m_buttonPos[i][1] = y;
        y -= incr;
        LOG_INFO("pos %d %f %f", i, x, y);
    }

}

void StartScreen::getTextPos()
{
    TextSystem &txtSys = App::g_app.textSystem();
    for(int i = 0; i < NUM_BUTTONS; ++i) {
        float w, h;
        txtSys.getStringSize(w, h, TextSystem::MEDIUM, m_texts[i]);
        m_textPos[i][0] = m_buttonPos[i][0] - w / 2.0f;
        m_textPos[i][1] = m_buttonPos[i][1] - h / 2.0f;
    }
}

int StartScreen::update(float delta)
{
    return 0;
}

void StartScreen::present()
{
    SimpleShaderProgram &program = App::g_app.program();
    TextSystem &textSys = App::g_app.textSystem();
    float *color;

    for(int i = 0; i < NUM_BUTTONS; ++i) {
        m_rect.draw(program, m_buttonPos[i], nullptr, nullptr, nullptr,
                    m_button.textureId(), nullptr);
        if(i == m_hoverButtonIdx) {
            color = m_hoverTextColor;
        } else if(i == m_pressedButtonIdx) {
            color = m_pressedTextColor;
        } else {
            color = m_normalTextColor;
        }
        textSys.drawString(program, m_texts[i], TextSystem::MEDIUM,
                    m_textPos[i], color);
    }
}

int StartScreen::processInput(const InputEvent &e)
{
    switch(e.m_type) {
    case InputEvent::ET_MOUSE_MOVE:
        processMouseMoveEvent(e.m_mouseMoveEvent);
        break;
    case InputEvent::ET_MOUSE_BUTTON:
        return processMouseButtonEvent(e.m_mouseButtonEvent);
    case InputEvent::ET_KEY:
        return processKeyEvent(e.m_keyEvent);
        break;
    default:
        LOG_WARN("Unknown event type: %d", static_cast<int>(e.m_type));
    }
    return 0;
}

void StartScreen::processMouseMoveEvent(const MouseMoveEvent &e)
{
    m_hoverButtonIdx = getButtonIdx(e.m_x, e.m_y);
    m_pressedButtonIdx = -1;
}

int StartScreen::processMouseButtonEvent(const MouseButtonEvent &e)
{
    if(e.m_button != GLFW_MOUSE_BUTTON_LEFT) {
        return 0;
    }

    if(e.m_button == GLFW_MOUSE_BUTTON_LEFT) {
        m_pressedButtonIdx = getButtonIdx(e.m_x, e.m_y);
        m_hoverButtonIdx = -1;
        if(m_pressedButtonIdx == BUTTON_START_GAME) {
            App::g_app.screenMgr().switchScreen(ScreenManager::SCREEN_GAME);
            return 1;
        } else if(m_pressedButtonIdx == BUTTON_EXIT) {
            return 2;
        }
    }
    return 0;
}

int StartScreen::processKeyEvent(const KeyEvent &e)
{
    return 0;
}

int StartScreen::getButtonIdx(float x, float y)
{
    float deltax = m_rect.width() / 2.0f;
    float deltay = m_rect.height() / 2.0f;
    for(int i = 0; i < NUM_BUTTONS; ++i) {
        float distx = x - m_buttonPos[i][0];
        if(distx < -deltax || distx > deltax) {
            continue;
        }

        float disty = y - m_buttonPos[i][1];
        if(disty >= -deltay && disty <= deltay) {
            return i;
        }
    }
    return -1;
}

} // end of namespace bot
