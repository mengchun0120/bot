#ifndef INCLUDE_BOT_WIDGET
#define INCLUDE_BOT_WIDGET

#include "misc/bot_constants.h"
#include "geometry/bot_rectangle.h"

namespace bot {

struct KeyEvent;
struct MouseMoveEvent;
struct MouseButtonEvent;

class Widget {
public:
    Widget();

    virtual ~Widget()
    {}

    bool init(float width, float height);

    virtual int processKeyEvent(const KeyEvent& event) = 0;

    virtual int processMouseMoveEvent(const MouseMoveEvent& event) = 0;

    virtual int processMouseButtonEvent(const MouseButtonEvent& event) = 0;

    virtual void setPos(float x, float y);

    virtual void present() = 0;

    virtual void onLostFocus() = 0;

    virtual void onMouseOut() = 0;

    bool containPos(float x, float y);

protected:
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    Rectangle m_rect;
    float m_left, m_right, m_top, m_bottom;
};

} // end of namespace bot

#endif
