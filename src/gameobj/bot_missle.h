#ifndef INCLUDE_BOT_MISSILE
#define INCLUDE_BOT_MISSILE

#include "gameobj/bot_gameobject.h"

namespace bot {

class Missile : public GameObject {
public:
    Missile();

    virtual ~Missile();

    virtual void present();

    virtual bool update(float delta, GameScreen& screen);

    virtual float getPosX() const;

    virtual float getPosY() const;

    virtual void setPos(float x, float y);

    virtual void setDirection(float directionX, float directionY);

    virtual float getCoverBreathX() const;

    virtual float getCoverBreathY() const;

    virtual bool isCollidable() const;

    virtual float getCollideBreathX() const;

    virtual float getCollideBreathY() const;

protected:
};

} // end of namespace

#endif
