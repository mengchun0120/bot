#ifndef INCLUDE_BOT_GAMEOBJECTTEMPLATE
#define INCLUDE_BOT_GAMEOBJECTTEMPLATE

#include <vector>
#include "bot_constants.h"

namespace bot {

class ComponentTemplate;

class GameObjectTemplate {
public:
    enum {
        MAX_PARTS = 4
    };

    struct Part {
        ComponentTemplate *m_component;
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    enum Type {
        BOT,
        TILE,
        BULLET,
        UNKNONWN
    };

    GameObjectTemplate()
    : m_baseComponent(nullptr)
    {}

    virtual ~GameObjectTemplate()
    {}

    Type getType() const
    {
        return m_type;
    }

    void setType(Type type)
    {
        m_type = type;
    }

    ComponentTemplate *getBaseComponent() const
    {
        return m_baseComponent;
    }

    void setBaseComponent(ComponentTemplate *c)
    {
        m_baseComponent = c;
    }

    int numParts() const
    {
        return static_cast<int>(m_parts.size());
    }

    void resizeParts(int size)
    {
        m_parts.resize(size);
    }

    Part &getPart(int idx)
    {
        return m_parts[idx];
    }

    const Part &getPart(int idx) const
    {
        return m_parts[idx];
    }

    void setPart(int idx, ComponentTemplate *c, float x, float y)
    {
        Part &p = m_parts[idx];
        p.m_component = c;
        p.m_pos[0] = x;
        p.m_pos[1] = y;
    }

private:
    Type m_type;
    ComponentTemplate *m_baseComponent;
    std::vector<Part> m_parts;
};

} // end of namespace bot

#endif