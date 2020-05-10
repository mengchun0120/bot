#ifndef INCLUDE_BOT_PARTICLE_SHADER_PROGRAM
#define INCLUDE_BOT_PARTICLE_SHADER_PROGRAM

#include "opengl/bot_opengl.h"
#include "opengl/bot_shader_program.h"

namespace bot {

class VertexArray;

class ParticleShaderProgram : public ShaderProgram {
public:
    ParticleShaderProgram();

    virtual ~ParticleShaderProgram();

    virtual bool init(const std::string& vertexShaderFile, const std::string& fragShaderFile);

    void setViewportSize(const float* viewportSize)
    {
        glUniform2fv(m_viewportSizeLocation, 1, viewportSize);
    }

    void setViewportOrigin(const float* viewportOrigin)
    {
        glUniform2fv(m_viewportOriginLocation, 1, viewportOrigin);
    }

    void setCurTime(float curTime)
    {
        glUniform1f(m_curTimeLocation, curTime);
    }

    void setAcceleration(float acceleration)
    {
        glUniform1f(m_accelerationLocation, acceleration);
    }

    void setParticleSize(float particleSize)
    {
        glUniform1f(m_particleSizeLocation, particleSize);
    }

    void setUseTex(bool useTex)
    {
        glUniform1i(m_useTexLocation, useTex ? 1 : 0);
    }

    void setTexture(int textureId);

    void bindData(VertexArray& va);

protected:
    void loadParam();

    int m_viewportSizeLocation;
    int m_viewportOriginLocation;
    int m_curTimeLocation;
    int m_accelerationLocation;
    int m_particleSizeLocation;
    int m_useTexLocation;
    int m_textureLocation;
    int m_positionLocation;
    int m_colorLocation;
    int m_directionLocation;
    int m_initSpeedLocation;
    int m_startTimeLocation;
};


} // end of namespace bot

#endif
