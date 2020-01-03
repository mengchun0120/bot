#ifndef INCLUDE_BOT_GAMEOBJECT
#define INCLUDE_BOT_GAMEOBJECT

#include <vector>
#include "bot_component.h"
#include "bot_gameobjecttemplate.h"

namespace bot {

class GameObject {
public:
    GameObject();

    GameObject(GameObjectTemplate *t, float x, float y);

    virtual ~GameObject();

    void init(GameObjectTemplate *t, float x, float y);

    int update(float delta);

    void present();

    const Component& getBase() const
    {
        return m_base;
    }

    Component& getBase()
    {
        return m_base;
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

private:
    GameObjectTemplate *m_template;
    Component m_base;
    int m_coverStartRow, m_coverEndRow;
    int m_coverStartCol, m_coverEndCol;
    std::vector<Component> m_parts;
};

} // end of namespace bot

#endif

