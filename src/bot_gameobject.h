#ifndef INCLUDE_BOT_GAMEOBJECT
#define INCLUDE_BOT_GAMEOBJECT

#include <vector>
#include "bot_component.h"
#include "bot_gameobjecttemplate.h"

namespace bot {

enum GameObjectFlag {
    GOBJ_FLAG_DRAWN = 0x00000001,
    GOBJ_FLAG_UPDATED = 0x00000002
};

class GameObject {
public:
    static GameObject* createFromTemplate(const GameObjectTemplate* t);

    static GameObject* createFromJson(const char* jsonFile);

    virtual ~GameObject();

    int update(float delta);

    void present();

    float getPosX() const
    {
        return m_base.getX();
    }

    float getPosY() const
    {
        return m_base.getY();
    }

    void setPos(float x, float y);

    void move(float deltaX, float deltaY);

    void setDirection(float directionX, float directionY);

    const Component& getBase() const
    {
        return m_base;
    }

    Component& getBase()
    {
        return m_base;
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

    int getNumParts() const
    {
        return static_cast<int>(m_parts.size());
    }

    const Component& getPartAt(int idx) const
    {
        return m_parts[idx];
    }

    Component& getPartAt(int idx)
    {
        return m_parts[idx];
    }

    float getCoverLeft() const
    {
        return m_base.getX() - m_coverBreathX;
    }

    float getCoverRight() const
    {
        return m_base.getX() + m_coverBreathX;
    }

    float getCoverTop() const
    {
        return m_base.getY() + m_coverBreathY;
    }

    float getCoverBottom() const
    {
        return m_base.getY() - m_coverBreathY;
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

    bool testFlag(GameObjectFlag flag)
    {
        return (m_flags & static_cast<int>(flag)) != 0;
    }

    int getFlags() const
    {
        return m_flags;
    }

private:
    GameObject();

private:
    float m_coverBreathX, m_coverBreathY;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    Component m_base;
    std::vector<Component> m_parts;
    int m_flags;
};

} // end of namespace bot

#endif
