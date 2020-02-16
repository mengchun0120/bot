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
        , m_template(t)
    {}

    virtual ~Ability()
    {}

    AbilityType getType() const
    {
        return m_template->getType();
    }

protected:
    const AbilityTemplate* m_template;
};

class MoveAbility: public Ability {
public:
    MoveAbility(const MoveAbilityTemplate* t)
        : Ability(t)
        , m_moving(false)
        , m_destX(0.0f)
        , m_destY(0.0f)
    {
    }

    virtual ~MoveAbility()
    {}

    float getSpeed() const
    {
        return static_cast<const MoveAbilityTemplate*>(m_template)->getSpeed();
    }

    bool isMoving() const
    {
        return m_moving;
    }

    void setMoving(bool moving)
    {
        m_moving = moving;
    }

    float getDestX() const
    {
        return m_destX;
    }

    void setDestX(float destX)
    {
        m_destX = destX;
    }

    float getDestY() const
    {
        return m_destY;
    }

    void setDestY(float destY)
    {
        m_destY = destY;
    }

protected:
    bool m_moving;
    float m_destX, m_destY;
};

class FireAbility: public Ability {
public:
    FireAbility(const FireAbilityTemplate* t, float componentX, float componentY);

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

    float getFireSpeed() const
    {
        return static_cast<const FireAbilityTemplate*>(m_template)->getFireSpeed();
    }

protected:
    Clock::time_point m_lastFireTime;
    float m_firePos[Constants::NUM_FLOATS_PER_POSITION];
};

class ExplodeAbility: public Ability {
public:
    ExplodeAbility(const ExplodeAbilityTemplate *t)
        : Ability(t)
    {}

    virtual ~ExplodeAbility()
    {}

    int getExplodePower() const
    {
        return static_cast<const ExplodeAbilityTemplate*>(m_template)->getExplodePower();
    }
};

} // end of namespace bot

#endif