#ifndef INCLUDE_BOT_GAMEOBJECT
#define INCLUDE_BOT_GAMEOBJECT

#include "structure/bot_doublelinkeditem.h"
#include "gameobj/bot_gameobjecttype.h"
#include "gameobj/bot_gameobjectflag.h"

namespace bot {

class GameScreen;

class GameObject: public DoubleLinkedItem {
public:
    GameObject(GameObjectType type)
        : m_type(type)
    {}

    virtual ~GameObject()
    {}

    GameObjectType getType() const
    {
        return m_type;
    }

    virtual void present() = 0;

    virtual bool update(float delta, GameScreen& screen) = 0;

    virtual float getPosX() const = 0;

    virtual float getPosY() const = 0;
    
    virtual void setPos(float x, float y) = 0;

    virtual void setDirection(float directionX, float directionY) = 0;

    virtual float getCoverBreathX() const = 0;

    virtual float getCoverBreathY() const = 0;

    virtual bool isCollidable() const = 0;

    virtual float getCollideBreathX() const = 0;

    virtual float getCollideBreathY() const = 0;

    float getCoverLeft() const
    {
        return getPosX() - getCoverBreathX();
    }

    float getCoverBottom() const
    {
        return getPosY() - getCoverBreathY();
    }

    float getCoverRight() const
    {
        return getPosX() + getCoverBreathX();
    }

    float getCoverTop() const
    {
        return getPosY() + getCoverBreathY();
    }

    float getCollideLeft() const
    {
        return getPosX() - getCollideBreathX();
    }

    float getCollideBottom() const
    {
        return getPosY() - getCollideBreathY();
    }

    float getCollideRight() const
    {
        return getPosX() + getCollideBreathX();
    }

    float getCollideTop() const
    {
        return getPosY() + getCollideBreathY();
    }

    int getCoverStartRow() const
    {
        return m_coverStartRow;
    }

    void setCoverStartRow(int startRow) 
    {
        m_coverStartRow = startRow;
    }

    int getCoverEndRow() const
    {
        return m_coverEndRow;
    }

    void setCoverEndRow(int endRow)
    {
        m_coverEndRow = endRow;
    }

    int getCoverStartCol() const
    {
        return m_coverStartCol;
    }

    void setCoverStartCol(int startCol)
    {
        m_coverStartCol = startCol;
    }

    int getCoverEndCol() const
    {
        return m_coverEndCol;
    }

    void setCoverEndCol(int endCol)
    {
        m_coverEndCol = endCol;
    }

    void clearFlag(GameObjectFlag flag)
    {
        m_flags &= (~static_cast<int>(flag));
    }

    void setFlag(GameObjectFlag flag)
    {
        m_flags |= static_cast<int>(flag);
    }

    bool testFlag(GameObjectFlag flag) const
    {
        return (m_flags & static_cast<int>(flag)) != 0;
    }

    int getFlags() const
    {
        return m_flags;
    }

    int getHP() const
    {
        return m_hp;
    }

    void setHP(int hp)
    {
        m_hp = hp;
    }

    int getSide() const
    {
        return m_side;
    }

    void setSide(int side)
    {
        m_side = side;
    }

protected:
    GameObjectType m_type;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    int m_flags;
    int m_hp;
    int m_side;
};

} // end of namespace bot

#endif
