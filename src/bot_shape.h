#ifndef INCLUDE_BOT_SHAPE
#define INCLUDE_BOT_SHAPE

#include "bot_vertexarray.h"

namespace bot {

class SimpleShaderProgram;

class Shape {
public:
    Shape();

    virtual ~Shape();

    virtual void draw(SimpleShaderProgram& program,
                      const float *pos, const float *direction,
                      const float *fillColor, const float *borderColor,
                      const unsigned int textureId, const float *texColor) = 0;

    const VertexArray& vertexArray() const
    {
        return m_vertexArray;
    }

protected:
    VertexArray m_vertexArray;
};

} // end of namespace bot

#endif
