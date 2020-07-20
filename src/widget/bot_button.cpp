#include "input/bot_input_event.h"
#include "opengl/bot_color.h"
#include "widget/bot_button.h"
#include "app/bot_app.h"

namespace bot {

Button::Button()
{
    const ButtonConfig& cfg = App::getInstance().getGameLib().getButtonConfig();
    m_textColor = cfg.getNormalTextColor();
    m_textPos[0] = 0.0f;
    m_textPos[1] = 0.0f;
}

bool Button::init(const Rectangle* rect, const std::string& text)
{
    Widget::init(rect);
    m_text = text;

    return true;
}

void Button::setPos(float x, float y)
{
    const TextSystem& textSys = App::getInstance().getTextSystem();

    Widget::setPos(x, y);

    float textWidth, textHeight;
    textSys.getStringSize(textWidth, textHeight, TEXT_SIZE_BIG, m_text);

    m_textPos[0] = m_pos[0] - textWidth / 2.0f;
    m_textPos[1] = m_pos[1] - textHeight / 2.0f;
}

int Button::processMouseMoveEvent(const MouseMoveEvent& event)
{
    const ButtonConfig& cfg = App::getInstance().getGameLib().getButtonConfig();
    m_textColor = cfg.getHoverTextColor();
    return 0;
}

int Button::processMouseButtonEvent(const MouseButtonEvent& event)
{
    if (event.m_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (event.m_action == GLFW_PRESS)
        {
            const ButtonConfig& cfg = App::getInstance().getGameLib().getButtonConfig();
            m_textColor = cfg.getPressTextColor();
        }
        else if (event.m_action == GLFW_RELEASE && m_actionFunc)
        {
            return m_actionFunc();
        }
    }

    return 0;
}

void Button::onMouseOut()
{
    const ButtonConfig& cfg = App::getInstance().getGameLib().getButtonConfig();
    m_textColor = cfg.getNormalTextColor();
}

void Button::present()
{
    if (!m_visible)
    {
        return;
    }

    App& app = App::getInstance();
    SimpleShaderProgram& program = app.getSimpleShaderProgram();
    const TextSystem& textSys = app.getTextSystem();
    const ButtonConfig& cfg = app.getGameLib().getButtonConfig();

    m_rect->draw(m_pos, nullptr, nullptr, nullptr, cfg.getTexture()->textureId(), nullptr);
    textSys.drawString(m_text, TEXT_SIZE_BIG, m_textPos, m_textColor->getColor());
}

} // end of namespace bot
