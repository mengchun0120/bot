#include "bot_log.h"
#include "bot_app.h"
#include "bot_ability.h"
#include "bot_componenttemplate.h"
#include "bot_component.h"

namespace bot {

Component::Component()
: m_template(nullptr)
, m_firstAbility(nullptr)
{}

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
        case AbilityTemplate::MOVE: {
            MoveAbility *a = new MoveAbility(static_cast<MoveAbilityTemplate*>(t));
            a->setDirection(1.0f, 0.0f);
            addAbility(a);
            break;
        }
        case AbilityTemplate::FIRE: {
            FireAbility *a = new FireAbility(static_cast<FireAbilityTemplate*>(t));
            addAbility(a);
            break;
        }
        case AbilityTemplate::EXPLODE: {
            ExplodeAbility *a = new ExplodeAbility(
                                        static_cast<ExplodeAbilityTemplate*>(t));
            addAbility(a);
            break;
        }
        default:
            LOG_ERROR("Invalid ability type: %d", static_cast<int>(t->getType()));
        }
    }
}

Ability *Component::getAbility(AbilityTemplate::Type type)
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
    Ability *t = getAbility(AbilityTemplate::MOVE);
    if(t) {
        direction = static_cast<MoveAbility*>(t)->getDirection();
    }
    rect.draw(App::g_app.program(), m_pos, direction, nullptr, nullptr,
              m_template->getTexture().textureId(), nullptr);
}

} // end of namespace bot
