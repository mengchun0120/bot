#ifndef INCLUDE_BOT_START_SCREEN
#define INCLUDE_BOT_START_SCREEN

#include <string>
#include "misc/bot_constants.h"
#include "screen/bot_screen.h"
#include "widget/bot_widget_group.h"

namespace bot {

class Texture;
class Rectangle;
class Color;
class App;
struct MouseMoveEvent;
struct MouseButtonEvent;
struct KeyEvent;

class StartScreen: public Screen {
public:
    StartScreen();

    virtual ~StartScreen();

    virtual bool init();

    virtual int update(float delta);

    virtual void present();

    virtual int processInput(const InputEvent &e);

private:
    int startGame();

    int loadGame();

    int showSettings();

    int exitApp();

private:
    enum {
        BUTTON_START_GAME = 0,
        BUTTON_LOAD_GAME,
        BUTTON_SETTINGS,
        BUTTON_EXIT,
        NUM_BUTTONS
    };

    WidgetGroup m_buttons;
    /*const Texture* m_button;
    const Rectangle* m_rect;
    const Color* m_normalTextColor;
    const Color* m_hoverTextColor;
    const Color* m_pressedTextColor;
    std::string m_texts[NUM_BUTTONS];
    float m_buttonPos[NUM_BUTTONS][Constants::NUM_FLOATS_PER_POSITION];
    float m_textPos[NUM_BUTTONS][Constants::NUM_FLOATS_PER_POSITION];
    int m_hoverButtonIdx;
    int m_pressedButtonIdx;*/
    float m_viewportOrigin[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif

