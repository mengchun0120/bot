#include <GL/glew.h>
#include "bot_simpleshaderprogram.h"
#include "bot_rectangle.h"

namespace bot {

Rectangle::Rectangle()
: Polygon()
, m_width(0)
, m_height(0)
{
}

Rectangle::~Rectangle()
{
}

bool Rectangle::init(float width0, float height0, bool hasTexCoord)
{
    float halfWidth = width0 / 2.0f;
    float halfHeight = height0 / 2.0f;
    const unsigned int NUM_VERTICES = 6;

    if(!hasTexCoord) {
        float vertices[] = {
            0.0f, 0.0f,
            halfWidth, halfHeight,
            -halfWidth, halfHeight,
            -halfWidth, -halfHeight,
            halfWidth, -halfHeight,
            halfWidth, halfHeight
        };

        if(!Polygon::init(vertices, NUM_VERTICES, false)) {
            return false;
        }

    } else {
        float vertices[] = {
            0.0f, 0.0f, 0.5f, 0.5f,
            halfWidth, halfHeight, 1.0f, 1.0f,
            -halfWidth, halfHeight, 0.0f, 1.0f,
            -halfWidth, -halfHeight, 0.0f, 0.0f,
            halfWidth, -halfHeight, 1.0f, 0.0f,
            halfWidth, halfHeight, 1.0f, 1.0f
        };

        if(!Polygon::init(vertices, NUM_VERTICES, true)) {
            return false;
        }
    }

    m_width = width0;
    m_height = height0;

    return true;
}

} // end of namespace bot
