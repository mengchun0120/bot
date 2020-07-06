#include <cmath>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "opengl/bot_color.h"
#include "opengl/bot_simple_shader_program.h"
#include "gameobj/bot_progress_ring.h"

namespace bot {

ProgressRing::ProgressRing()
    : m_frontColor(nullptr)
    , m_backColor(nullptr)
    , m_maxIdx(0)
{
}

bool ProgressRing::init(const Color* frontColor, const Color* backColor, float radius, int numEdges)
{
    m_frontColor = frontColor;
    m_backColor = backColor;
    m_maxIdx = numEdges - 1;

    int numFloatsPerTriangle = Constants::NUM_FLOATS_PER_POSITION * 3;
    int numFloats = numEdges * numFloatsPerTriangle;
    float* vertices = new float[numFloats];
    float delta = 2.0f * Constants::PI / numEdges;
    float theta = delta;
    float prevX = 0.0f, prevY = radius;

    for (int i = 0, k = 0; i < numEdges; ++i, k += numFloatsPerTriangle, theta += delta)
    {
        vertices[k] = 0.0f;
        vertices[k + 1] = 0.0f;
        vertices[k + 2] = prevX;
        vertices[k + 3] = prevY;
        prevX = sin(theta) * radius;
        prevY = cos(theta) * radius;
        vertices[k + 4] = prevX;
        vertices[k + 5] = prevY;
    }

    bool ret = m_vertices.load(vertices, numEdges * 3, Constants::POSITION_SIZE, 0);
    delete[] vertices;

    return ret;
}

void ProgressRing::draw(SimpleShaderProgram& program, const float* pos, float percentage) const
{
    program.setUseObjRef(true);
    program.setObjRef(pos);
    program.setPosition(m_vertices, false);
    program.setUseColor(true);
    program.setUseDirection(false);

    int finishedIdx = static_cast<int>(percentage * m_maxIdx);

    if (finishedIdx < 0)
    {
        finishedIdx = 0;
    }
    else if(finishedIdx > m_maxIdx)
    {
        finishedIdx = m_maxIdx;
    }

    int finishedVertices = (finishedIdx + 1) * 3;

    program.setColor(m_backColor->getColor());
    glDrawArrays(GL_TRIANGLES, 0, finishedVertices);

    if (finishedVertices < m_vertices.numVertices())
    {
        program.setColor(m_frontColor->getColor());
        glDrawArrays(GL_TRIANGLES, finishedVertices, m_vertices.numVertices() - finishedVertices);
    }
}

} // end of namespace bot
