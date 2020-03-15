#include "misc/bot_log.h"
#include "misc/bot_utils.h"
#include "app/bot_app.h"
#include "screen/bot_gamescreen.h"
#include "gameobj/bot_ability.h"
#include "gameobj/bot_component.h"

namespace bot {

Component::Component()
: m_template(nullptr)
, m_firstAbility(nullptr)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
    m_direction[0] = 1.0f;
    m_direction[1] = 0.0f;
}

Component::Component(const ComponentTemplate *t, float x, float y, float directionX, float directionY)
{
    init(t, x, y, directionX, directionY);
}

Component::~Component()
{
    Ability *cur, *next;
    for(cur = m_firstAbility; cur; cur = next) {
        next = static_cast<Ability*>(cur->getNext());
        delete cur;
    }
}

void Component::init(const ComponentTemplate *t, float x, float y, float directionX, float directionY)
{
    m_template = t;
    m_pos[0] = x;
    m_pos[1] = y;
    m_direction[0] = directionX;
    m_direction[1] = directionY;
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
    rect.draw(App::g_app.program(), m_pos, m_direction, nullptr, nullptr,
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
    m_direction[0] = directionX;
    m_direction[1] = directionY;
    FireAbility* ability = static_cast<FireAbility*>(getAbility(ABILITY_FIRE));
    if (ability) {
        ability->rotateFirePos(m_pos[0], m_pos[1], directionX, directionY);
    }
}

bool Component::updateFireAbility(float delta, const Clock::time_point& t, GameScreen& screen)
{
    FireAbility* fireAbility = static_cast<FireAbility*>(getAbility(ABILITY_FIRE));
    if (!fireAbility) {
        return false;
    }

    if (!fireAbility->shouldFire(t)) {
        return false;
    }

    bool explodeOnCreation = false;

    float x = fireAbility->getFirePosX();
    float y = fireAbility->getFirePosY();
    const GameObjectTemplate& bulletTemplate = fireAbility->getBulletTemplate();

    if (screen.checkOutsideMap(x, y)) {
        explodeOnCreation = true;
    }
    else {
        float collideLeft = x - bulletTemplate.getCollideBreathX();
        float collideRight = x + bulletTemplate.getCollideBreathX();
        float collideBottom = y - bulletTemplate.getCollideBreathY();
        float collideTop = y + bulletTemplate.getCollideBreathY();

    }





    return true;
}

} // end of namespace bot
