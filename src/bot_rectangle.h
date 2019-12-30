#ifndef INCLUDE_BOT_RECTANGLE
#define INCLUDE_BOT_RECTANGLE

#include "bot_polygon.h"

namespace bot {

class Rectangle: public Polygon {
public:
    Rectangle();

    virtual ~Rectangle();

    bool init(float width0, float height0, bool hasTexCoord);

    float width() const
    {
        return m_width;
    }

    float height() const
    {
        return m_height;
    }

protected:
    float m_width;
    float m_height;
};

} // end of namespace bot

#endif
