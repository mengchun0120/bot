#ifndef INCLUDE_BOT_SHOOTABILITY
#define INCLUDE_BOT_SHOOTABILITY

#include <chrono>
#include "gametemplate/bot_shootabilitytemplate.h"
#include "gameobj/bot_ability.h"

namespace bot {

class MissileTemplate;

class ShootAbility : public Ability {
public:
    typedef std::chrono::high_resolution_clock ShootClock;
    typedef ShootClock::time_point ShootTime;

    ShootAbility(const ShootAbilityTemplate* t);

    virtual ~ShootAbility()
    {}

    const float* getShootPos() const
    {
        return static_cast<const float*>(m_shootPos);
    }

    float getShootPosX() const
    {
        return m_shootPos[0];
    }

    float getShootPosY() const
    {
        return m_shootPos[1];
    }

    bool isShootingEnabled() const
    {
        return m_shootingEnabled;
    }

    void enableShooting(bool enabled)
    {
        m_shootingEnabled = enabled;
    }

    void setShootTime();

    const ShootAbilityTemplate* getTemplate() const
    {
        return static_cast<const ShootAbilityTemplate*>(m_template);
    }

    const MissileTemplate* getMissileTemplate() const
    {
        return getTemplate()->getMissileTemplate();
    }

    bool shouldFire() const;

    void shiftShootPos(float deltaX, float deltaY);

    void setShootPosDirection(float referenceX, float referenceY, float directionX, float directionY);

protected:
    ShootTime m_lastShootTime;
    float m_shootingEnabled;
    float m_shootDirection[Constants::NUM_FLOATS_PER_POSITION];
    float m_shootPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
