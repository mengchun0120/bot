#ifndef INCLUDE_BOT_POLYGON
#define INCLUDE_BOT_POLYGON

#include "geometry/bot_shape.h"

namespace bot {

class Polygon: public Shape {
public:
    Polygon();

    virtual ~Polygon();

    bool init(const float *vertices, unsigned int numVertices, bool hasTexCoord);

    virtual void draw(SimpleShaderProgram& program,
                      const float *pos, const float *direction,
                      const float *fillColor, const float *borderColor,
                      unsigned int textureId, const float *texColor) const;
};

} // end of namespace bot

#endif
