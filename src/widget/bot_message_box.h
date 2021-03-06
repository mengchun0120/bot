#ifndef INCLUDE_BOT_MESSAGE_BOX
#define INCLUDE_BOT_MESSAGE_BOX

#include "misc/bot_constants.h"
#include "widget/bot_button.h"
#include "widget/bot_widget_group.h"

namespace bot {

class Graphics;
class TextSystem;
class Rectangle;
class MessageBoxConfig;
class ButtonConfig;
struct InputEvent;

class MessageBox {
    enum ButtonIdx {
        BUTTON_OK,
        BUTTON_CANCEL
    };

public:
    enum Option {
        OPTION_OK,
        OPTION_OK_CANCEL
    };

    MessageBox();
    
    ~MessageBox()
    {}

    void init(const MessageBoxConfig* cfg, const ButtonConfig* buttonCfg, const TextSystem& textSys,
              Option opt, const std::string& msg);

    void setOKAction(const Button::ActionFunc& func);

    void setCancelAction(const Button::ActionFunc& func);

    void show(Graphics& g);

    int processInput(const InputEvent& e);

private:
    const MessageBoxConfig* m_cfg;
    std::string m_msg;
    float m_msgPos[Constants::NUM_FLOATS_PER_POSITION];
    WidgetGroup m_buttons;
};

} // end of namespace bot

#endif
