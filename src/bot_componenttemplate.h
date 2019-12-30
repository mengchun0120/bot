#ifndef INCLUDE_BOT_COMPONENTTEMPLATE
#define INCLUDE_BOT_COMPONENTTEMPLATE

#include "bot_abilitytemplate.h"

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

    Texture &getTexture()
    {
        return *m_texture;
    }

    Rectangle &getRect()
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

    int getHP() const
    {
        return m_hp;
    }

    void setHP(int hp)
    {
        m_hp = hp;
    }

    AbilityTemplate *firstAbility()
    {
        return m_firstAbility;
    }

    int numAbilities() const
    {
        return m_numAbilities;
    }

    void addAbility(AbilityTemplate *ability);

private:
    Texture *m_texture;
    Rectangle *m_rect;
    int m_hp;
    int m_numAbilities;
    AbilityTemplate *m_firstAbility;
};

} // end of namespace bot

#endif