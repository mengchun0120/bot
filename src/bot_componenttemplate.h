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
        , m_hp(0)
        , m_numAbilities(0)
        , m_firstAbility(nullptr)
        , m_collideBreathX(0.0f)
        , m_collideBreathY(0.0f)
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

    int getNumAbilities() const
    {
        return m_numAbilities;
    }

    void addAbility(AbilityTemplate *ability);

    float getCollideBreathX() const
    {
        return m_collideBreathX;
    }

    float getCollideBreathY() const
    {
        return m_collideBreathY;
    }

    void setCollideBreathX(float breathX)
    {
        m_collideBreathX = breathX;
    }

    void setCollideBreathY(float breathY)
    {
        m_collideBreathY = breathY;
    }

private:
    Texture *m_texture;
    Rectangle *m_rect;
    int m_hp;
    int m_numAbilities;
    AbilityTemplate *m_firstAbility;
    float m_collideBreathX;
    float m_collideBreathY;
};

} // end of namespace bot

#endif