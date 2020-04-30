#ifndef INCLUDE_BOT_MOVEABILITY
#define INCLUDE_BOT_MOVEABILITY

#include "gametemplate/bot_moveabilitytemplate.h"
#include "gameobj/bot_ability.h"
#include "misc/bot_log.h"

namespace bot {

class MoveAbility : public Ability {
public:
    MoveAbility(const MoveAbilityTemplate* t)
        : Ability(t)
        , m_moving(false)
        , m_hasDest(false)
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
        LOG_INFO("Move %d", (moving ? 1 : 0));
        m_moving = moving;
    }

    float getDestX() const
    {
        return m_destX;
    }

    void setDest(float destX, float destY)
    {
        m_destX = destX;
        m_destY = destY;
        m_hasDest = true;
    }

    float getDestY() const
    {
        return m_destY;
    }

    bool hasDest() const
    {
        return m_hasDest;
    }

    void setHasDest(bool hasDest)
    {
        m_hasDest = hasDest;
    }

    void checkDest(float x, float y);

protected:
    bool m_moving;
    bool m_hasDest;
    float m_destX, m_destY;
};


} // end of namespace bot

#endif
