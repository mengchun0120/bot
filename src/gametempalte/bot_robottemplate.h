#ifndef INCLUDE_BOT_ROBOTTEMPLATE
#define INCLUDE_BOT_ROBOTTEMPLATE

namespace bot {

class RobotTemplate {
    enum {
        MAX_PARTS = 4
    };

    struct Part {
        ComponentTemplate* m_component;
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
        float m_direction[Constants::NUM_FLOATS_PER_POSITION];
    };

    GameObjectTemplate();

    virtual ~GameObjectTemplate()
    {}

    ComponentTemplate* getBaseComponent() const
    {
        return m_baseComponent;
    }

    void setBaseComponent(ComponentTemplate* c)
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

    Part& getPart(int idx)
    {
        return m_parts[idx];
    }

    const Part& getPart(int idx) const
    {
        return m_parts[idx];
    }

    void setPart(int idx, ComponentTemplate* c, float x, float y, float directionX, float directionY)
    {
        Part& p = m_parts[idx];
        p.m_component = c;
        p.m_pos[0] = x;
        p.m_pos[1] = y;
        p.m_direction[0] = directionX;
        p.m_direction[1] = directionY;
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

private:
    ComponentTemplate* m_baseComponent;
    std::vector<Part> m_parts;
    float m_coverBreathX;
    float m_coverBreathY;
    int m_hp;
    float m_collideBreathX;
    float m_collideBreathY;
};

} // end of namespace bot

#endif
