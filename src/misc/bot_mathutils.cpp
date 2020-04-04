#include <cmath>
#include "misc/bot_mathutils.h"

namespace bot {

void rotate(float& x, float& y, float directionX, float directionY)
{
    float x1 = x * directionX - y * directionY;
    float y1 = x * directionY + y * directionX;
    x = x1;
    y = y1;
}

void getDirection(float& directionX, float& directionY, float srcX, float srcY, float dstX, float dstY)
{
    float deltaX = dstX - srcX;
    float deltaY = dstY - srcY;
    float dist = sqrt(deltaX * deltaX + deltaY * deltaY);
    directionX = deltaX / dist;
    directionY = deltaY / dist;
}

} // end of namespace bot
