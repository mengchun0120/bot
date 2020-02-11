#ifndef INCLUDE_BOT_ABILITY
#define INCLUDE_BOT_ABILITY

#include "bot_utils.h"
#include "bot_linkeditem.h"
#include "bot_abilitytemplate.h"

namespace bot {

class Component;

class Ability: public LinkedItem {
public:
    Ability(const AbilityTemplate* t)
        : LinkedItem()
    {}

    virtual ~Ability()
    {}
protected:
    AbilityTemplate* m_template;
};

class MoveAbility: public Ability {
public:
    MoveAbility(const MoveAbilityTemplate* t)
        : Ability(t)
    {
    }

    virtual ~MoveAbility()
    {}

    float getSpeed() const
    {
        return static_cast<MoveAbilityTemplate*>(m_template)->getSpeed();
    }
};

class FireAbility: public Ability {
public:
    FireAbility(const FireAbilityTemplate* t, const Component& component)
        : Ability(t)
    {
        m_lastFireTime = Clock::now();

        m_firePos[0] = component. + t->getFirePosX();
        m_firePos[1] = componentY + t->getFirePosY();
        rotate(m_fireDirection[0], m_fireDirection[1], componentDirectionX, componentDirectionY);
    }

    virtual ~FireAbility()
    {}

    const float *getFirePos() const
    {
        return static_cast<const float*>(m_firePos);
    }

    float getFirePosX() const
    {
        return m_firePos[0];
    }

    float getFirePosY() const
    {
        return m_firePos[1];
    }

    void setFirePos(float x, float y)
    {
        m_firePos[0] = x;
        m_firePos[1] = y;
    }

    void moveFirePos(float deltaX, float deltaY)
    {
        m_firePos[0] += deltaX;
        m_firePos[1] += deltaY;
    }

    const float* getFireDirection() const
    {
        return static_cast<const float *>(m_fireDirection);
    }

    float getFireDirectionX() const
    {
        return m_fireDirection[0];
    }

    float getFireDirectionY() const
    {
        return m_fireDirection[1];
    }

    void setFireDirection(float directionX, float directionY)
    {
        m_fireDirection[0] = directionX;
        m_fireDirection[1] = directionY;
    }

    float getFireSpeed() const
    {
        return static_cast<FireAbilityTemplate*>(m_template)->getFireSpeed();
    }

protected:
    Clock::time_point m_lastFireTime;
    float m_firePos[Constants::NUM_FLOATS_PER_POSITION];
    float m_fireDirection[Constants::NUM_FLOATS_PER_POSITION];
};

class ExplodeAbility: public Ability {
public:
    ExplodeAbility(const ExplodeAbilityTemplate *t)
        : Ability(ABILITY_EXPLODE)
        , m_template(t)
    {}

    virtual ~ExplodeAbility()
    {}

    int getExplodePower() const
    {
        return m_template->getExplodePower();
    }

protected:
    const ExplodeAbilityTemplate *m_template;
};

} // end of namespace bot

#endif