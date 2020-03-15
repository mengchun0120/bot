#ifndef INCLUDE_BOT_COMPONENTTEMPLATE
#define INCLUDE_BOT_COMPONENTTEMPLATE

namespace bot {

class Texture;
class Rectangle;
class AbilityTemplate;

class ComponentTemplate {
public:
    ComponentTemplate()
        : m_texture(nullptr)
        , m_rect(nullptr)
        , m_numAbilities(0)
        , m_firstAbility(nullptr)
    {}

    virtual ~ComponentTemplate();

    Texture& getTexture()
    {
        return *m_texture;
    }

    const Texture& getTexture() const
    {
        return *m_texture;
    }

    Rectangle& getRect()
    {
        return *m_rect;
    }

    const Rectangle& getRect() const
    {
        return *m_rect;
    }

    void setTexture(Texture *texture)
    {
        m_texture = texture;
    }

    void setRect(Rectangle *rect)
    {
        m_rect = rect;
    }

    const AbilityTemplate* firstAbility() const
    {
        return m_firstAbility;
    }

    AbilityTemplate* firstAbility()
    {
        return m_firstAbility;
    }

    int getNumAbilities() const
    {
        return m_numAbilities;
    }

    void addAbility(AbilityTemplate *ability);

private:
    Texture *m_texture;
    Rectangle *m_rect;
    int m_numAbilities;
    AbilityTemplate *m_firstAbility;
};

} // end of namespace bot

#endif