#include "bot_log.h"
#include "bot_app.h"
#include "bot_ability.h"
#include "bot_component.h"

namespace bot {

Component::Component()
: m_template(nullptr)
, m_firstAbility(nullptr)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

Component::Component(ComponentTemplate *t, float x, float y)
{
    init(t, x, y);
}

Component::~Component()
{
    Ability *cur, *next;
    for(cur = m_firstAbility; cur; cur = next) {
        next = static_cast<Ability*>(cur->getNext());
        delete cur;
    }
}

void Component::init(ComponentTemplate *t, float x, float y)
{
    m_template = t;
    m_pos[0] = x;
    m_pos[1] = y;
    initAbilities();
}

void Component::initAbilities()
{
    AbilityTemplate *t;
    for(t = m_template->firstAbility(); t; t = static_cast<AbilityTemplate*>(t->getNext())) {
        switch(t->getType()) {
        case ABILITY_MOVE: {
            MoveAbility* a = new MoveAbility(static_cast<MoveAbilityTemplate*>(t));
            addAbility(a);
            break;
        }
        case ABILITY_FIRE: {
            FireAbility *a = new FireAbility(static_cast<FireAbilityTemplate*>(t), m_pos[0], m_pos[1]);
            addAbility(a);
            break;
        }
        case ABILITY_EXPLODE: {
            ExplodeAbility *a = new ExplodeAbility(static_cast<ExplodeAbilityTemplate*>(t));
            addAbility(a);
            break;
        }
        default:
            LOG_ERROR("Invalid ability type: %d", static_cast<int>(t->getType()));
        }
    }
}

Ability* Component::getAbility(AbilityType type) const
{
    Ability *t;
    for(t = m_firstAbility; t; t = static_cast<Ability*>(t->getNext())) {
        if(t->getType() == type) {
            return t;
        }
    }
    return nullptr;
}

void Component::addAbility(Ability *a)
{
    a->setNext(m_firstAbility);
    m_firstAbility = a;
}

void Component::present()
{
    Rectangle &rect = m_template->getRect();
    const float *direction = nullptr;
    Ability *ability = getAbility(ABILITY_MOVE);
    if(ability) {
        direction = static_cast<MoveAbility*>(ability)->getDirection();
    }
    rect.draw(App::g_app.program(), m_pos, direction, nullptr, nullptr,
              m_template->getTexture().textureId(), nullptr);
}

} // end of namespace bot
