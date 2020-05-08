#include "gametemplate/bot_moveabilitytemplate.h"

namespace bot {

MoveAbilityTemplate::MoveAbilityTemplate(float speed)
    : AbilityTemplate(ABILITY_MOVE)
    , m_speed(speed)
{}

MoveAbilityTemplate::~MoveAbilityTemplate()
{}

} // end of namespace bot
