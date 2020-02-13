#ifndef INCLUDE_BOT_COMPONENT
#define INCLUDE_BOT_COMPONENT

#include <vector>
#include "bot_constants.h"
#include "bot_abilitytemplate.h"
#include "bot_componenttemplate.h"

namespace bot {

class Ability;

class Component {
public:
    Component();

    Component(const ComponentTemplate *t, float x, float y, float directionX, float directionY);

    virtual ~Component();

    void init(const ComponentTemplate *t, float x, float y, float directionX, float directionY);

    float getX() const
    {
        return m_pos[0];
    }

    float getY() const
    {
        return m_pos[1];
    }

    const float *getPos() const
    {
        return static_cast<const float*>(m_pos);
    }

    float *getPos()
    {
        return static_cast<float *>(m_pos);
    }

    void present();

    void setPos(float x, float y);

    void move(float deltaX, float deltaY);

    void setDirection(float directionX, float directionY);

    float getDirectionX() const
    {
        return m_direction[0];
    }

    float getDirectionY() const
    {
        return m_direction[1];
    }

    Ability* getAbility(AbilityType type) const;

    Ability* getFirstAbility() const
    {
        return m_firstAbility;
    }

private:
    void initAbilities();

    void addAbility(Ability* ability);

private:
    const ComponentTemplate* m_template;
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    Ability* m_firstAbility;
};

} // end of namespace bot

#endif
