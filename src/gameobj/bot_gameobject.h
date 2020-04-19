#ifndef INCLUDE_BOT_GAMEOBJECT
#define INCLUDE_BOT_GAMEOBJECT

#include "structure/bot_doublelinkeditem.h"
#include "gameobj/bot_gameobjecttype.h"
#include "gameobj/bot_gameobjectflag.h"

namespace bot {

class GameScreen;
class SimpleShaderProgram;

class GameObject: public DoubleLinkedItem {
public:
    GameObject(GameObjectType type)
        : DoubleLinkedItem()
        , m_type(type)
        , m_coverStartRow(0)
        , m_coverEndRow(0)
        , m_coverStartCol(0)
        , m_coverEndCol(0)
        , m_flags(0)
    {}

    virtual ~GameObject()
    {}

    GameObjectType getType() const
    {
        return m_type;
    }

    virtual void present(SimpleShaderProgram& program) = 0;

    virtual bool update(float delta, GameScreen& screen) = 0;

    virtual float getPosX() const = 0;

    virtual float getPosY() const = 0;
    
    virtual void setPos(float x, float y) = 0;

    virtual float getCoverBreathX() const = 0;

    virtual float getCoverBreathY() const = 0;

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

    void setCoverRect(int startRow, int endRow, int startCol, int endCol)
    {
        m_coverStartRow = startRow;
        m_coverEndRow = endRow;
        m_coverStartCol = startCol;
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

protected:
    GameObjectType m_type;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    int m_flags;
};

} // end of namespace bot

#endif
