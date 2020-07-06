#ifndef INCLUDE_BOT_DASHBOARD_TEMPLATE
#define INCLUDE_BOT_DASHBOARD_TEMPLATE

namespace bot {

class DashboardTemplate {
public:
    DashboardTemplate()
        : m_spacingX(0.0f)
        , m_marginBottom(0.0f)
        , m_ringRadius(0.0f)
    {}

    ~DashboardTemplate()
    {}

    float getSpacingX() const
    {
        return m_spacingX;
    }

    void setSpacingX(float spacingX)
    {
        m_spacingX = spacingX;
    }

    float getMarginBottom() const
    {
        return m_marginBottom;
    }

    void setMarginBottom(float marginBottom)
    {
        m_marginBottom = marginBottom;
    }

    float getRingRadius() const
    {
        return m_ringRadius;
    }

    void setRingRadius(float ringRadius)
    {
        m_ringRadius = ringRadius;
    }

private:
    float m_spacingX;
    float m_marginBottom;
    float m_ringRadius;
};

} // end of namespace bot

#endif
