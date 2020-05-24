#ifndef INCLUDE_BOT_MOVE_ABILITY
#define INCLUDE_BOT_MOVE_ABILITY

#include "gametemplate/bot_move_ability_template.h"
#include "gameobj/bot_ability.h"

namespace bot {

class MoveAbility : public Ability {
public:
    MoveAbility(const MoveAbilityTemplate* t)
        : Ability(t)
        , m_moving(false)
    {
    }

    virtual ~MoveAbility()
    {}

    float getSpeed() const
    {
        return static_cast<const MoveAbilityTemplate*>(m_template)->getSpeed();
    }

    bool isMoving() const
    {
        return m_moving;
    }

    void setMoving(bool moving)
    {
        m_moving = moving;
    }

protected:
    bool m_moving;
};


} // end of namespace bot

#endif
