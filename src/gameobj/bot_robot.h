#ifndef INCLUDE_BOT_ROBOT
#define INCLUDE_BOT_ROBOT

#include "gameobj/bot_gameobject.h"

namespace bot {

class Robot : public GameObject {
public:
	Robot();

	virtual ~Robot();

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
    FireAbility* getFireAbility() const;

    void updateFireAbility(float delta, const Clock::time_point& t, GameScreen& screen);

    void updateFireAbilityComponent(Component& component, float delta, const Clock::time_point& t, GameScreen& screen);

    int updateMoveAbility(float delta, GameScreen& screen);
};

} // end of namespace bot

#endif
