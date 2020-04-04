#ifndef INCLUDE_BOT_MATHUTILS
#define INCLUDE_BOT_MATHUTILS

#include "misc/bot_constants.h"

namespace bot {

template <typename T>
T clamp(const T& t, const T& min, const T& max)
{
    if (t < min) {
        return min;
    }
    else if (t > max) {
        return max;
    }

    return t;
}

inline float safeDivide(float x, float y)
{
    return (y > Constants::FLOAT_ZERO || y < -Constants::FLOAT_ZERO) ? (x / y) : (x / Constants::FLOAT_ZERO);
}

void fillColor(float* color, int r, int g, int b, int alpha);

void rotate(float& x, float& y, float directionX, float directionY);

void getDirection(float& directionX, float& directionY, float srcX, float srcY, float dstX, float dstY);

} // end of namespace bot

#endif
