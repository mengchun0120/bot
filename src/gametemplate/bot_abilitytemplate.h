#ifndef INCLUDE_BOT_ABILITYTEMPLATE
#define INCLUDE_BOT_ABILITYTEMPLATE

#include "gametemplate/bot_abilitytype.h"

namespace bot {

class AbilityTemplate {
public:
    AbilityTemplate(AbilityType type)
        : m_type(type)
    {}

    virtual ~AbilityTemplate()
    {}

    AbilityType getType() const
    {
        return m_type;
    }

protected:
    AbilityType m_type;
};

} // end of namespace bot

#endif