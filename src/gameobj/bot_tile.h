#ifndef INCLUDE_BOT_TILE
#define INCLUDE_BOT_TILE

#include "misc/bot_constants.h"
#include "gameobj/bot_gameobject.h"

namespace bot {

class Tile : public GameObject {
public:
    Tile();

    virtual ~Tile();

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
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
