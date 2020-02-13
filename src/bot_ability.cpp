#include "bot_ability.h"

namespace bot {

FireAbility::FireAbility(const FireAbilityTemplate* t, float componentX, float componentY)
    : Ability(t)
{
    m_lastFireTime = Clock::now();
    m_firePos[0] = componentX + t->getFirePosX();
    m_firePos[1] = componentY + t->getFirePosY();
}

} // end of namespace bot