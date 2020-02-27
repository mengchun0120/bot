#ifndef INCLUDE_BOT_COLLIDE
#define INCLUDE_BOT_COLLIDE

namespace bot {

bool checkObjCollision(float& newDelta, float x1, float y1, float collideBreathX1, float collideBreathY1, 
                       float speedX, float speedY, float x2, float y2, float collideBreathX2, float collideBreathY2,
                       float delta);

} // end of namespace bot

#endif