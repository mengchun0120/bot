#ifndef INLCUDE_BOT_MOVEABILITYTEMPLATE
#define INLCUDE_BOT_MOVEABILITYTEMPLATE

#include "gametemplate/bot_abilitytemplate.h"

namespace bot {

class MoveAbilityTemplate : public AbilityTemplate {
public:
    MoveAbilityTemplate(float speed);

    virtual ~MoveAbilityTemplate();

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    float m_speed;
};

} // end of namespace bot

#endif
