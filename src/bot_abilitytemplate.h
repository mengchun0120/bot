#ifndef INCLUDE_BOT_ABILITYTEMPLATE
#define INCLUDE_BOT_ABILITYTEMPLATE

#include <string>
#include "bot_linkeditem.h"
#include "bot_constants.h"

namespace bot {

enum AbilityType {
    ABILITY_MOVE,
    ABILITY_FIRE,
    ABILITY_EXPLODE
};

class GameObjectTemplate;

class AbilityTemplate: public LinkedItem {
public:
    AbilityTemplate(AbilityType type)
        : LinkedItem()
        , m_type(type)
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

class MoveAbilityTemplate: public AbilityTemplate {
public:
    MoveAbilityTemplate(float speed)
    : AbilityTemplate(ABILITY_MOVE)
    , m_speed(speed)
    {}

    virtual ~MoveAbilityTemplate()
    {}

    float getSpeed() const
    {
        return m_speed;
    }

protected:
    float m_speed;
};

class FireAbilityTemplate: public AbilityTemplate {
public:
    FireAbilityTemplate(float speed, float fireX, float fireY, float fireDirectionX,
                        float fireDirectionY, const std::string bulletName)
    : AbilityTemplate(ABILITY_FIRE)
    , m_speed(speed)
    , m_bulletName(bulletName)
    , m_bulletTemplate(nullptr)
    {
        m_firePos[0] = fireX;
        m_firePos[1] = fireY;
        m_fireDirection[0] = fireDirectionX;
        m_fireDirection[1] = fireDirectionY;
    }

    virtual ~FireAbilityTemplate()
    {}

    const float *getFirePos() const
    {
        return static_cast<const float *>(m_firePos);
    }

    float getFirePosX() const
    {
        return m_firePos[0];
    }

    float getFirePosY() const
    {
        return m_firePos[1];
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

    const std::string &getBulletName() const
    {
        return m_bulletName;
    }

    const GameObjectTemplate &getBulletTemplate() const
    {
        return *m_bulletTemplate;
    }

    float getFireSpeed() const
    {
        return m_speed;
    }

    void setBullet(GameObjectTemplate *bullet)
    {
        m_bulletTemplate = bullet;
    }

protected:
    float m_speed;
    float m_firePos[Constants::NUM_FLOATS_PER_POSITION];
    float m_fireDirection[Constants::NUM_FLOATS_PER_POSITION];
    std::string m_bulletName;
    GameObjectTemplate *m_bulletTemplate;
};

class ExplodeAbilityTemplate: public AbilityTemplate {
public:
    ExplodeAbilityTemplate(int explodePower)
    : AbilityTemplate(ABILITY_EXPLODE)
    , m_explodePower(explodePower)
    {}

    virtual ~ExplodeAbilityTemplate()
    {}

    int getExplodePower() const
    {
        return m_explodePower;
    }

protected:
    int m_explodePower;
};

} // end of namespace bot

#endif