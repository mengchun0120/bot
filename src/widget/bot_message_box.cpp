#include "input/bot_input_event.h"
#include "widget/bot_message_box.h"
#include "app/bot_app.h"

namespace bot {

MessageBox::MessageBox()
{
    m_msgPos[0] = 0.0f;
    m_msgPos[1] = 0.0f;
}

void MessageBox::init(Option opt, const std::string& msg)
{
    const App& app = App::getInstance();
    const TextSystem& textSys = app.getTextSystem();
    const MessageBoxConfig& cfg = app.getGameLib().getMessageBoxConfig();
    float boxWidth = cfg.getBoxRect()->width();
    float boxX = cfg.getBoxLeft();
    float textWidth, textHeight;
    
    m_msg = msg;
    textSys.getStringSize(textWidth, textHeight, TEXT_SIZE_BIG, m_msg);
    m_msgPos[0] = boxX + (boxWidth - textWidth) / 2.0f;
    m_msgPos[1] = cfg.getBoxTop() - cfg.getMsgMarginY() - textHeight;

    int buttonCount = opt == OPTION_OK ? 1 : 2;
    float buttonWidth = cfg.getButtonRect()->width();
    float spacing = cfg.getButtonSpacing();
    float buttonX = boxX + (boxWidth - buttonCount * buttonWidth - (buttonCount - 1) * spacing) / 2.0f;
    const std::vector<std::string>& texts = cfg.getButtonTexts();

    m_buttons.init(buttonCount);
    for (int i = 0; i < buttonCount; ++i)
    {
        Button* button = new Button();
        button->init(cfg.getButtonRect(), texts[i]);
        button->setPos(buttonX, cfg.getButtonY());
        m_buttons.setWidget(i, button);
        buttonX += buttonWidth + spacing;
    }
}

void MessageBox::setOKAction(const Button::ActionFunc& func)
{
    Button& okButton = static_cast<Button&>(m_buttons.getWidget(BUTTON_OK));
    okButton.setActionFunc(func);
}

void MessageBox::setCancelAction(const Button::ActionFunc& func)
{
    if (m_buttons.getWidgetCount() < 1)
    {
        return;
    }

    Button& cancelButton = static_cast<Button&>(m_buttons.getWidget(BUTTON_CANCEL));
    cancelButton.setActionFunc(func);
}

int MessageBox::processInput(const InputEvent& e)
{
    return m_buttons.processInput(e);
}

void MessageBox::show()
{
    const App& app = App::getInstance();
    const TextSystem& textSys = app.getTextSystem();
    const MessageBoxConfig& cfg = app.getGameLib().getMessageBoxConfig();

    cfg.getBoxRect()->draw(cfg.getBoxPos(), nullptr, cfg.getBoxFillColor(), cfg.getBoxBorderColor(), 0, nullptr);
    textSys.drawString(m_msg, TEXT_SIZE_BIG, m_msgPos, cfg.getTextColor()->getColor());
    m_buttons.present();
}

} // end of namespace bot
