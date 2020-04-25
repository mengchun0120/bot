#ifndef INCLUDE_BOT_GAMEOBJECTTEMPLATE
#define INCLUDE_BOT_GAMEOBJECTTEMPLATE

#include "gameobj/bot_gameobjecttype.h"
#include "gameobj/bot_gameobjectflag.h"

namespace bot {

class GameObjectTemplate {
public:
    GameObjectTemplate(GameObjectType type);

    GameObjectTemplate(GameObjectType type, float coverBreathX, float coverBreathY, 
                       float collideBreathX, float collideBreathY, int flags);

    virtual ~GameObjectTemplate()
    {}

    GameObjectType getType() const
    {
        return m_type;
    }

    float getCoverBreathX() const
    {
        return m_coverBreathX;
    }

    void setCoverBreathX(float breathX)
    {
        m_coverBreathX = breathX;
    }

    float getCoverBreathY() const
    {
        return m_coverBreathY;
    }

    void setCoverBreathY(float breathY)
    {
        m_coverBreathY = breathY;
    }

    float getCollideBreathX() const
    {
        return m_collideBreathX;
    }

    void setCollideBreathX(float collideBreathX)
    {
        m_collideBreathX = collideBreathX;
    }

    float getCollideBreathY() const
    {
        return m_collideBreathY;
    }

    void setCollideBreathY(float collideBreathY)
    {
        m_collideBreathY = collideBreathY;
    }

    int getFlags() const
    {
        return m_flags;
    }

    void clearFlag(GameObjectFlag flag)
    {
        m_flags &= ~(static_cast<int>(flag));
    }

    void setFlag(GameObjectFlag flag)
    {
        m_flags |= static_cast<int>(flag);
    }

protected:
    GameObjectType m_type;
    float m_coverBreathX;
    float m_coverBreathY;
    float m_collideBreathX;
    float m_collideBreathY;
    int m_flags;
};

} // end of namespace bot

#endif