#ifndef INCLUDE_BOT_PROGRESS_RING
#define INCLUDE_BOT_PROGRESS_RING

#include "opengl/bot_vertex_array.h"

namespace bot {

class Color;
class SimpleShaderProgram;

class ProgressRing {
public:
    ProgressRing();

    ~ProgressRing()
    {}

    bool init(const Color* frontColor, const Color* backColor, float radius, int numEdges);

    void draw(SimpleShaderProgram& program, const float* pos, float percentage) const;

private:
    const Color* m_frontColor;
    const Color* m_backColor;
    VertexArray m_vertices;
    int m_maxIdx;
};

} // end of namespace bot

#endif
