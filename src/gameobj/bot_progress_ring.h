#ifndef INCLUDE_BOT_PROGRESS_RING
#define INCLUDE_BOT_PROGRESS_RING

#include "opengl/bot_vertex_array.h"

namespace bot {

class Color;
class SimpleShaderProgram;

class ProgressRing {
public:
    static ProgressRing* create(const rapidjson::Value& elem);

    ProgressRing();

    ~ProgressRing()
    {}

    bool init(const rapidjson::Value& elem);

    bool init(const Color* frontColor, const Color* backColor, float radius, int numEdges);

    void draw(const float* pos, float percentage) const;

private:
    const Color* m_frontColor;
    const Color* m_backColor;
    VertexArray m_vertices;
    int m_maxIdx;
};

} // end of namespace bot

#endif
