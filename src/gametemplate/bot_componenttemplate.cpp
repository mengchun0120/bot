#include "gameobj/bot_abilitytemplate.h"
#include "gameobj/bot_componenttemplate.h"

namespace bot {

ComponentTemplate::~ComponentTemplate()
{
    AbilityTemplate *cur, *next;

    for(cur = m_firstAbility; cur; cur = next) {
        next = static_cast<AbilityTemplate*>(cur->getNext());
        delete cur;
    }
}

void ComponentTemplate::addAbility(AbilityTemplate *ability)
{
    ability->setNext(m_firstAbility);
    m_firstAbility = ability;
    ++m_numAbilities;
}


} // end of namespace bot