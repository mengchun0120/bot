#include "bot_simpleshaderprogram.h"
#include "bot_polygon.h"

namespace bot {

Polygon::Polygon()
: Shape()
{
}

Polygon::~Polygon()
{
}

bool Polygon::init(const float *vertices, unsigned int numVertices, bool hasTexCoord)
{
    return m_vertexArray.load(vertices, numVertices, hasTexCoord);
}

void Polygon::draw(SimpleShaderProgram& program,
                   const float *pos, const float *direction,
                   const float *fillColor, const float *borderColor,
                   unsigned int textureId, const float *texColor)
{
    if(pos) {
        program.setUseObjRef(true);
        program.setObjRef(pos);
    } else {
        program.setUseObjRef(false);
    }

    program.setPosition(m_vertexArray);
    program.setUseColor(textureId == 0);

    if(direction) {
        program.setUseDirection(true);
        program.setDirection(direction);
    } else {
        program.setUseDirection(false);
    }

    if(textureId == 0) {
        if(fillColor) {
            program.setColor(fillColor);
            glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexArray.numVertices());
        }

        if(borderColor) {
            program.setColor(borderColor);
            glDrawArrays(GL_LINE_LOOP, 1, m_vertexArray.numVertices()-2);
        }
    } else {
        program.setTexture(textureId);

        if(texColor) {
            program.setUseTexColor(true);
            program.setTexColor(texColor);
        } else {
            program.setUseTexColor(false);
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertexArray.numVertices());
    }
}


} // end of namespace bot
