#include "bot_collide.h"

namespace bot {

inline float safeDivide(float x, float y)
{
    static const float ZERO = 1.0e-6f;
    return (y > ZERO || y < -ZERO) ? (x / y) : (x / ZERO);
}

inline float convetionFloat(float x)
{
    const float MIN_DIST = 0.01f;
    return x > MIN_DIST ? x - MIN_DIST : 0.0f;
}

bool checkCornerCollision(float& newDelta, float nearDistX, float nearDistY, float farDistX, float farDistY,
                          float absSpeedX, float absSpeedY, float delta)
{
    float movDistX = absSpeedX * delta;
    float movDistY = absSpeedY * delta;

    if (movDistX < nearDistX || movDistY < nearDistY) {
        return false;
    }

    float distYSpeedX = nearDistY * absSpeedX;
    float distXSpeedY = nearDistX * absSpeedY;
    if (distYSpeedX <= distXSpeedY) {
        if (distXSpeedY > farDistY* absSpeedX) {
            return false;
        }

        newDelta = safeDivide(convetionFloat(nearDistX), absSpeedX);
    }
    else {
        if (distYSpeedX > farDistX* absSpeedY) {
            return false;
        }

        newDelta = safeDivide(convetionFloat(nearDistY), absSpeedY);
    }

    return true;
}

bool checkMiddleCollision(float& newDelta, float distX, float farDistUpY, float farDistDownY, float absSpeedX,
                          float speedY, float delta)
{
    float movDistX = absSpeedX * delta;

    if (movDistX < distX) {
        return false;
    }

    float distXSpeedY = distX * speedY;
    if (speedY > 0.0f) {
        if (distXSpeedY > farDistUpY * absSpeedX) {
            return false;
        }
    }
    else if (speedY < 0.0f) {
        if (-distXSpeedY > farDistDownY * absSpeedX) {
            return false;
        }
    }

    newDelta = safeDivide(convetionFloat(distX), absSpeedX);

    return true;
}

bool checkObjCollision(float& newDelta, float x1, float y1, float collideBreathX1, float collideBreathY1,
                       float speedX1, float speedY1, float x2, float y2, float collideBreathX2, float collideBreathY2,
                       float delta)
{
    float dx = x1 - x2;
    float sumBreathX = collideBreathX1 + collideBreathX2;
    float x12 = dx - sumBreathX;
    float x21 = -dx - sumBreathX;
    bool onLeft = false, onRight = false;
    
    if (x12 >= 0.0f) {
        if (speedX1 >= 0.0f) {
            return false;
        }

        onLeft = true;
    }
    else if (x21 >= 0.0f) {
        if (speedX1 <= 0.0f) {
            return false;
        }

        onRight = true;
    }
    
    float dy = y1 - y2;
    float sumBreathY = collideBreathY1 + collideBreathY2;
    float y12 = dy - sumBreathY;
    float y21 = -dy - sumBreathY;
    bool onTop = false, onBottom = false;

    if (y12 >= 0.0f) {
        if (speedY1 >= 0.0f) {
            return false;
        }

        onBottom = true;
    }
    else if (y21 >= 0.0f) {
        if (speedY1 <= 0.0f) {
            return false;
        }

        onTop = true;
    }

    bool collide = false;
    if (onLeft) {
        if (onTop) {
            collide = checkCornerCollision(newDelta, x12, y21, -x21, -y12, -speedX1, speedY1, delta);
        }
        else if (onBottom) {
            collide = checkCornerCollision(newDelta, x12, y12, -x21, -y21, -speedX1, -speedY1, delta);
        }
        else {
            collide = checkMiddleCollision(newDelta, x12, -y12, -y21, -speedX1, speedY1, delta);
        }
    } 
    else if (onRight) {
        if (onTop) {
            collide = checkCornerCollision(newDelta, x21, y21, -x12, -y12, speedX1, speedY1, delta);
        }
        else if (onBottom) {
            collide = checkCornerCollision(newDelta, x21, y12, -x12, -y21, speedX1, -speedY1, delta);
        }
        else {
            collide = checkMiddleCollision(newDelta, x21, -y12, -y21, speedX1, speedY1, delta);
        }
    }
    else {
        if (onTop) {
            collide = checkMiddleCollision(newDelta, y21, -x12, -x21, speedY1, speedX1, delta);
        }
        else if(onBottom) {
            collide = checkMiddleCollision(newDelta, y12, -x12, -x21, -speedY1, speedX1, delta);
        }
        else {
            collide = true;
            newDelta = 0.0f;
        }
    }

    return collide;
}

} // end of namespace bot
