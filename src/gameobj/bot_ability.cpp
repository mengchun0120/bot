#include "gameobj/bot_ability.h"

namespace bot {

FireAbility::FireAbility(const FireAbilityTemplate* t, float componentX, float componentY)
    : Ability(t)
    , m_firing(false)
{
    m_lastFireTime = Clock::now();
    m_firePos[0] = componentX + t->getFirePosX();
    m_firePos[1] = componentY + t->getFirePosY();
}

void FireAbility::startFiring()
{
    m_firing = true;
}

void FireAbility::stopFiring()
{
    m_firing = false;
}

void FireAbility::rotateFirePos(float referenceX, float referenceY, float directionX, float directionY)
{
    const FireAbilityTemplate* t = static_cast<const FireAbilityTemplate*>(m_template);
    float x = t->getFirePosX();
    float y = t->getFirePosY();
    rotate(x, y, directionX, directionY);
    x += referenceX;
    y += referenceY;
    m_firePos[0] = x;
    m_firePos[1] = y;
}

bool FireAbility::shouldFire(const Clock::time_point& t) const
{
    using namespace std::chrono;
    float dur = static_cast<float>(duration_cast<milliseconds>(t - m_lastFireTime).count());
    return dur <= getFireSpeed();
}

} // end of namespace bot
