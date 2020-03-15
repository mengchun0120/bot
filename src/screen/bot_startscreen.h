#ifndef INCLUDE_BOT_STARTSCREEN
#define INCLUDE_BOT_STARTSCREEN

#include "misc/bot_constants.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "screen/bot_screen.h"

namespace bot {

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
    void setViewportOrigin();

    void getButtonPos();

    void getTextPos();

    void processMouseMoveEvent(const MouseMoveEvent &e);

    int processMouseButtonEvent(const MouseButtonEvent &e);

    int processKeyEvent(const KeyEvent &e);

    int getButtonIdx(float x, float y);

private:
    enum {
        BUTTON_START_GAME = 0,
        BUTTON_LOAD_GAME,
        BUTTON_SETTINGS,
        BUTTON_EXIT,
        NUM_BUTTONS
    };
    std::string m_texts[NUM_BUTTONS];
    Texture m_button;
    Rectangle m_rect;
    float m_buttonPos[NUM_BUTTONS][Constants::NUM_FLOATS_PER_POSITION];
    float m_textPos[NUM_BUTTONS][Constants::NUM_FLOATS_PER_POSITION];
    float m_normalTextColor[Constants::NUM_FLOATS_COLOR];
    float m_hoverTextColor[Constants::NUM_FLOATS_COLOR];
    float m_pressedTextColor[Constants::NUM_FLOATS_COLOR];
    int m_hoverButtonIdx;
    int m_pressedButtonIdx;
};

} // end of namespace bot

#endif

