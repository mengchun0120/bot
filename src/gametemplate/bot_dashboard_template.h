#ifndef INCLUDE_BOT_DASHBOARD_TEMPLATE
#define INCLUDE_BOT_DASHBOARD_TEMPLATE

namespace bot {

class Rectangle;
class Texture;
class Color;

class DashboardTemplate {
public:
    DashboardTemplate()
        : m_hpRect(nullptr)
        , m_hpTexture(nullptr)
        , m_hpTextColor(nullptr)
        , m_hpIconX(0.0f)
        , m_hpTextX(0.0f)
        , m_goldRect(nullptr)
        , m_goldTexture(nullptr)
        , m_goldTextColor(nullptr)
        , m_goldIconX(0.0f)
        , m_goldTextX(0.0f)
        , m_effectSpacingX(0.0f)
        , m_effectStartX(0.0f)
        , m_effectRingRadius(0.0f)
        , m_headerTopMargin(0.0f)
    {}

    ~DashboardTemplate()
    {}

    const Rectangle* getHPRect() const
    {
        return m_hpRect;
    }

    void setHPRect(const Rectangle* rect)
    {
        m_hpRect = rect;
    }

    const Texture* getHPTexture() const
    {
        return m_hpTexture;
    }

    void setHPTexture(const Texture* texture)
    {
        m_hpTexture = texture;
    }

    const Color* getHPTextColor() const
    {
        return m_hpTextColor;
    }

    void setHPTextColor(const Color* color)
    {
        m_hpTextColor = color;
    }

    const Rectangle* getGoldRect() const
    {
        return m_goldRect;
    }

    void setGoldRect(const Rectangle* rect)
    {
        m_goldRect = rect;
    }

    const Texture* getGoldTexture() const
    {
        return m_goldTexture;
    }

    void setGoldTexture(const Texture* texture)
    {
        m_goldTexture = texture;
    }

    const Color* getGoldTextColor() const
    {
        return m_goldTextColor;
    }

    void setGoldTextColor(const Color* color)
    {
        m_goldTextColor = color;
    }

    float getEffectSpacingX() const
    {
        return m_effectSpacingX;
    }

    void setEffectSpacingX(float spacingX)
    {
        m_effectSpacingX = spacingX;
    }

    float getEffectStartX() const
    {
        return m_effectStartX;
    }

    void setEffectStartX(float x)
    {
        m_effectStartX = x;
    }

    float getEffectRingRadius() const
    {
        return m_effectRingRadius;
    }

    void setEffectRingRadius(float ringRadius)
    {
        m_effectRingRadius = ringRadius;
    }

    float getHPIconX() const
    {
        return m_hpIconX;
    }

    void setHPIconX(float x)
    {
        m_hpIconX = x;
    }

    float getHPTextX() const
    {
        return m_hpTextX;
    }

    void setHPTextX(float x)
    {
        m_hpTextX = x;
    }

    float getGoldIconX() const
    {
        return m_goldIconX;
    }

    void setGoldIconX(float x)
    {
        m_goldIconX = x;
    }

    float getGoldTextX() const
    {
        return m_goldTextX;
    }

    void setGoldTextX(float x)
    {
        m_goldTextX = x;
    }

    float getHeaderTopMargin() const
    {
        return m_headerTopMargin;
    }

    void setHeaderTopMargin(float margin)
    {
        m_headerTopMargin = margin;
    }

private:
    const Rectangle* m_hpRect;
    const Texture* m_hpTexture;
    const Color* m_hpTextColor;
    float m_hpIconX;
    float m_hpTextX;
    const Rectangle* m_goldRect;
    const Texture* m_goldTexture;
    const Color* m_goldTextColor;
    float m_goldIconX;
    float m_goldTextX;
    float m_effectSpacingX;
    float m_effectStartX;
    float m_effectRingRadius;
    float m_headerTopMargin;
};

} // end of namespace bot

#endif
