#ifndef INCLUDE_BOT_GAMEOBJECTTEMPLATE
#define INCLUDE_BOT_GAMEOBJECTTEMPLATE

#include "gameobj/bot_gameobjectflag.h"

namespace bot {

class GameObjectTemplate {
public:
    GameObjectTemplate();

    GameObjectTemplate(float coverBreathX, float coverBreathY, float collideBreathX, float collideBreathY,
                       int hp, int flags);

    virtual ~GameObjectTemplate()
    {}

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

    int getHP() const
    {
        return m_hp;
    }

    void setHP(int hp)
    {
        m_hp = hp;
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
    float m_coverBreathX;
    float m_coverBreathY;
    float m_collideBreathX;
    float m_collideBreathY;
    int m_hp;
    int m_flags;
};

} // end of namespace bot

#endif