#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "input/bot_input_event.h"
#include "screen/bot_start_screen.h"
#include "app/bot_app.h"

namespace bot {

StartScreen::StartScreen(App* app)
    : m_app(app)
    , m_button(nullptr)
    , m_rect(nullptr)
    , m_normalTextColor(nullptr)
    , m_hoverTextColor(nullptr)
    , m_pressedTextColor(nullptr)
    , m_hoverButtonIdx(-1)
    , m_pressedButtonIdx(-1)
{
    m_texts[0] = "Start Game";
    m_texts[1] = "Load Game";
    m_texts[2] = "Settings";
    m_texts[3] = "Exit";
}

StartScreen::~StartScreen()
{
}

bool StartScreen::init()
{
    const GameTemplateLib& gameLib = m_app->getGameTemplateLib();
    
    m_button = gameLib.getTextureByName("button");
    if (!m_button)
    {
        LOG_ERROR("Failed to load texture button");
        return false;
    }

    m_rect = gameLib.getRectByName("button");
    if (!m_rect)
    {
        LOG_ERROR("Failed to load rectangle button");
        return false;
    }

    m_normalTextColor = gameLib.getColorByName("normal_text_color");
    if (!m_normalTextColor)
    {
        LOG_ERROR("Failed to load normal_text_color");
        return false;
    }

    m_hoverTextColor = gameLib.getColorByName("hover_text_color");
    if (!m_hoverTextColor)
    {
        LOG_ERROR("Failed to load hover_text_color");
        return false;
    }

    m_pressedTextColor = gameLib.getColorByName("pressed_text_color");
    if (!m_pressedTextColor)
    {
        LOG_ERROR("Failed to load pressed_text_color");
        return false;
    }

    setViewportOrigin();
    getButtonPos();
    getTextPos();

    return true;
}

void StartScreen::setViewportOrigin()
{
    float viewportOrigin[] = {m_app->getViewportWidth() / 2.0f, m_app->getViewportHeight() / 2.0f};
    m_app->getSimpleShaderProgram().setViewportOrigin(viewportOrigin);
}

void StartScreen::getButtonPos()
{
    float x = m_app->getViewportWidth() / 2.0f;
    const float SPACING = 20.0f;
    float y = (m_app->getViewportHeight() + static_cast<float>(m_rect->height()) * (NUM_BUTTONS + 1) +
               SPACING * (NUM_BUTTONS - 1)) / 2.0f;
    float incr = SPACING + m_rect->height();

    for (int i = 0; i < NUM_BUTTONS; ++i) 
    {
        m_buttonPos[i][0] = x;
        m_buttonPos[i][1] = y;
        y -= incr;
        LOG_DEBUG("pos %d %f %f", i, x, y);
    }

}

void StartScreen::getTextPos()
{
    const TextSystem& txtSys = m_app->getTextSystem();
    for (int i = 0; i < NUM_BUTTONS; ++i) 
    {
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
    SimpleShaderProgram& program = m_app->getSimpleShaderProgram();
    program.use();

    const TextSystem& textSys = m_app->getTextSystem();
    const float *color;

    for (int i = 0; i < NUM_BUTTONS; ++i) {
        m_rect->draw(program, m_buttonPos[i], nullptr, nullptr, nullptr, m_button->textureId(), nullptr);
        
        if(i == m_hoverButtonIdx) 
        {
            color = m_hoverTextColor->getColor();
        } 
        else if(i == m_pressedButtonIdx) 
        {
            color = m_pressedTextColor->getColor();
        }
        else 
        {
            color = m_normalTextColor->getColor();
        }
        
        textSys.drawString(program, m_texts[i], TextSystem::MEDIUM, m_textPos[i], color);
    }
}

int StartScreen::processInput(const InputEvent& e)
{
    switch(e.m_type) 
    {
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
    if(e.m_button != GLFW_MOUSE_BUTTON_LEFT) 
    {
        return 0;
    }

    if(e.m_button == GLFW_MOUSE_BUTTON_LEFT) 
    {
        m_pressedButtonIdx = getButtonIdx(e.m_x, e.m_y);
        m_hoverButtonIdx = -1;
        if(m_pressedButtonIdx == BUTTON_START_GAME) 
        {
            m_app->getScreenManager().switchScreen(ScreenManager::SCREEN_GAME);
            return 1;
        } 
        else if(m_pressedButtonIdx == BUTTON_EXIT) 
        {
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
    float deltax = m_rect->width() / 2.0f;
    float deltay = m_rect->height() / 2.0f;

    for(int i = 0; i < NUM_BUTTONS; ++i) 
    {
        float distx = x - m_buttonPos[i][0];
        if(distx < -deltax || distx > deltax) 
        {
            continue;
        }

        float disty = y - m_buttonPos[i][1];
        if(disty >= -deltay && disty <= deltay) 
        {
            return i;
        }
    }

    return -1;
}

} // end of namespace bot
