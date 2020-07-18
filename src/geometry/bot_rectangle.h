#ifndef INCLUDE_BOT_RECTANGLE
#define INCLUDE_BOT_RECTANGLE

#include <rapidjson/document.h>
#include "geometry/bot_polygon.h"

namespace bot {

class Rectangle: public Polygon {
public:
    static Rectangle* create(const rapidjson::Value& elem);

    Rectangle();

    virtual ~Rectangle();

    bool init(const rapidjson::Value& elem);

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
