#ifndef INCLUDE_BOT_GOODIE_TEMPLATE
#define INCLUDE_BOT_GOODIE_TEMPLATE

#include "gameobj/bot_goodie_type.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

class Rectangle;
class Texture;
class Color;
class ProgressRing;

class GoodieTemplate : public GameObjectTemplate {
public:
    GoodieTemplate()
        : GameObjectTemplate(GAME_OBJ_TYPE_GOODIE)
        , m_goodieType(GOODIE_UNKNOWN)
        , m_rect(nullptr)
        , m_texture(nullptr)
        , m_ring(nullptr)
        , m_duration(0.0f)
    {}

    virtual ~GoodieTemplate()
    {}

    GoodieType getGoodieType() const
    {
        return m_goodieType;
    }

    void setGoodieType(GoodieType goodieType)
    {
        m_goodieType = goodieType;
    }

    const Rectangle* getRect() const
    {
        return m_rect;
    }

    void setRect(const Rectangle* rect)
    {
        m_rect = rect;
    }

    const Texture* getTexture() const
    {
        return m_texture;
    }

    void setTexture(const Texture* texture)
    {
        m_texture = texture;
    }

    const ProgressRing* getProgressRing() const
    {
        return m_ring;
    }

    void setProgressRing(const ProgressRing* ring)
    {
        m_ring = ring;
    }

    float getDuration() const
    {
        return m_duration;
    }

    void setDuration(float duration)
    {
        m_duration = duration;
    }

    bool isEffect() const
    {
        return m_duration > 0.0f;
    }

    float getWeight() const
    {
        return m_weight;
    }

    void setWeight(float weight)
    {
        m_weight = weight;
    }

private:
    GoodieType m_goodieType;
    const Rectangle* m_rect;
    const Texture* m_texture;
    const ProgressRing* m_ring;
    float m_duration;
    float m_weight;
};

} // end of namespace bot

#endif