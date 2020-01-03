#ifndef INCLUDE_BOT_COLOR
#define INCLUDE_BOT_COLOR

#include "bot_constants.h"

namespace bot {

class Color {
public:
    Color()
    {
        m_color[0] = 0.0f;
        m_color[1] = 0.0f;
        m_color[2] = 0.0f;
        m_color[3] = 0.0f;
    }

    ~Color()
    {}

    void set(int red, int green, int blue, int alpha)
    {
        m_color[0] = red / 255.0f;
        m_color[1] = green / 255.0f;
        m_color[2] = blue / 255.0f;
        m_color[3] = alpha / 255.0f;
    }

    float *color()
    {
        return m_color;
    }

private:
    float m_color[Constants::NUM_FLOATS_COLOR];
};

} // end of namespace bot

#endif
