#include "bot_log.h"
#include "bot_app.h"
#include "bot_ability.h"
#include "bot_utils.h"
#include "bot_component.h"

namespace bot {

Component::Component()
: m_template(nullptr)
, m_firstAbility(nullptr)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

Component::Component(const ComponentTemplate *t, float x, float y)
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

void Component::init(const ComponentTemplate *t, float x, float y)
{
    m_template = t;
    m_pos[0] = x;
    m_pos[1] = y;
    initAbilities();
}

void Component::initAbilities()
{
    const AbilityTemplate *t;
    for(t = m_template->firstAbility(); t; t = static_cast<const AbilityTemplate*>(t->getNext())) {
        switch(t->getType()) {
        case ABILITY_MOVE: {
            MoveAbility* a = new MoveAbility(static_cast<const MoveAbilityTemplate*>(t));
            addAbility(a);
            break;
        }
        case ABILITY_FIRE: {
            FireAbility *a = new FireAbility(static_cast<const FireAbilityTemplate*>(t), m_pos[0], m_pos[1]);
            addAbility(a);
            break;
        }
        case ABILITY_EXPLODE: {
            ExplodeAbility *a = new ExplodeAbility(static_cast<const ExplodeAbilityTemplate*>(t));
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
    const Rectangle& rect = m_template->getRect();
    const float *direction = nullptr;
    const Ability* ability = getAbility(ABILITY_MOVE);
    if(ability) {
        direction = static_cast<const MoveAbility*>(ability)->getDirection();
    }
    rect.draw(App::g_app.program(), m_pos, direction, nullptr, nullptr,
              m_template->getTexture().textureId(), nullptr);
}

void Component::setPos(float x, float y)
{
    float deltaX = x - m_pos[0];
    float deltaY = y - m_pos[1];
    move(deltaX, deltaY);
}

void Component::move(float deltaX, float deltaY)
{
    m_pos[0] += deltaX;
    m_pos[1] += deltaY;
    FireAbility* ability = static_cast<FireAbility*>(getAbility(ABILITY_FIRE));
    if (ability) {
        ability->moveFirePos(deltaX, deltaY);
    }
}

void Component::setDirection(float directionX, float directionY)
{
    for (Ability* ability = m_firstAbility; ability; ability = static_cast<Ability*>(ability->getNext())) {
        switch (ability->getType()) {
            case ABILITY_MOVE: {
                MoveAbility* moveAbility = static_cast<MoveAbility*>(ability);
                moveAbility->setDirection(directionX, directionY);
                break;
            }
            case ABILITY_FIRE: {
                FireAbility* fireAbility = static_cast<FireAbility*>(ability);
                const FireAbilityTemplate* t = fireAbility->getTemplate();
                float fireDirectionX = t->getFireDirectionX();
                float fireDirectionY = t->getFireDirectionY();
                rotate(fireDirectionX, fireDirectionY, directionX, directionY);
                fireAbility->setFireDirection(fireDirectionX, fireDirectionY);
                break;
            }
        }
    }
}

} // end of namespace bot
