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

class GameObject: public DoubleLinkedItem {
public:
    GameObject(const GameObjectTemplate* t);

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
        return m_template->getCoverBreathX();
    }

    float getCoverBreathY() const
    {
        return m_template->getCoverBreathY();
    }

    int getNumComponents() const
    {
        return static_cast<int>(m_components.size());
    }

    const Component& getComponent(int idx) const
    {
        return m_components[idx];
    }

    Component& getComponent(int idx)
    {
        return m_components[idx];
    }

    float getCoverLeft() const
    {
        return m_base.getX() - m_template->getCoverBreathX();
    }

    float getCoverRight() const
    {
        return m_base.getX() + m_template->getCoverBreathX();
    }

    float getCoverTop() const
    {
        return m_base.getY() + m_template->getCoverBreathY();
    }

    float getCoverBottom() const
    {
        return m_base.getY() - m_template->getCoverBreathY();
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

    int getHP() const
    {
        return m_hp;
    }

    void setHP(int hp)
    {
        m_hp = hp;
    }

    float getCollideBreathX() const
    {
        return m_template->getCollideBreathX();
    }
    
    float getCollideBreathY() const
    {
        return m_template->getCollideBreathY();
    }

    float getCollideLeft() const
    {
        return m_base.getX() - m_template->getCollideBreathX();
    }

    float getCollideRight() const
    {
        return m_base.getX() + m_template->getCollideBreathX();
    }

    float getCollideBottom() const
    {
        return m_base.getY() - m_template->getCollideBreathY();
    }

    float getCollideTop() const
    {
        return m_base.getY() + m_template->getCollideBreathY();
    }

private:
    const GameObjectTemplate* m_template;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    Component m_base;
    std::vector<Component> m_components;
    int m_flags;
    int m_hp;
};

} // end of namespace bot

#endif
