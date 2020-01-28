#ifndef INCLUDE_BOT_ABILITY
#define INCLUDE_BOT_ABILITY

#include "bot_utils.h"
#include "bot_linkeditem.h"
#include "bot_abilitytemplate.h"

namespace bot {

class Ability: public LinkedItem {
public:
    Ability(AbilityType type)
        : LinkedItem()
        , m_type(type)
    {}

    virtual ~Ability()
    {}

    AbilityType getType() const
    {
        return m_type;
    }

protected:
    AbilityType m_type;
};

class MoveAbility: public Ability {
public:
    MoveAbility(const MoveAbilityTemplate *t)
        : Ability(ABILITY_MOVE)
        , m_template(t)
    {
        m_direction[0] = 1.0f;
        m_direction[1] = 0.0f;
    }

    virtual ~MoveAbility()
    {}

    float getSpeed() const
    {
        return m_template->getSpeed();
    }

    const float *getDirection() const
    {
        return static_cast<const float *>(m_direction);
    }

    void setDirection(float directionX, float directionY)
    {
        m_direction[0] = directionX;
        m_direction[1] = directionY;
    }

    float getDirectionX() const
    {
        return m_direction[0];
    }

    float getDirectionY() const
    {
        return m_direction[1];
    }

protected:
    const MoveAbilityTemplate *m_template;
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
};

class FireAbility: public Ability {
public:
    FireAbility(const FireAbilityTemplate *t, float componentX, float componentY)
        : Ability(ABILITY_FIRE)
        , m_template(t)
    {
        m_lastFireTime = Clock::now();
        m_firePos[0] = componentX + m_template->getFirePosX();
        m_firePos[1] = componentY + m_template->getFirePosY();
        m_fireDirection[0] = m_template->getFireDirectionX();
        m_fireDirection[1] = m_template->getFireDirectionY();
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

    const float *getFireDirection() const
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

    void setFireDirection(float x, float y)
    {
        m_fireDirection[0] = x;
        m_fireDirection[1] = y;
    }

    float getFireSpeed() const
    {
        return m_template->getFireSpeed();
    }

    const FireAbilityTemplate* getTemplate() const
    {
        return m_template;
    }

protected:
    const FireAbilityTemplate *m_template;
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