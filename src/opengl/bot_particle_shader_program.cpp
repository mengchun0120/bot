#include "misc/bot_constants.h"
#include "opengl/bot_vertex_array.h"
#include "opengl/bot_particle_shader_program.h"

namespace bot {

ParticleShaderProgram::ParticleShaderProgram()
    : m_viewportSizeLocation(-1)
    , m_viewportOriginLocation(-1)
    , m_curTimeLocation(-1)
    , m_accelerationLocation(-1)
    , m_particleSizeLocation(-1)
    , m_useTexLocation(-1)
    , m_positionLocation(-1)
    , m_colorLocation(-1)
    , m_directionLocation(-1)
    , m_initSpeedLocation(-1)
    , m_startTimeLocation(-1)
    , m_textureLocation(-1)
{

}

ParticleShaderProgram::~ParticleShaderProgram()
{
}

bool ParticleShaderProgram::init(const std::string& vertexShaderFile, const std::string& fragShaderFile)
{
    if (!ShaderProgram::init(vertexShaderFile, fragShaderFile))
    {
        return false;
    }

    loadParam();

    return true;
}

void ParticleShaderProgram::loadParam()
{
    m_viewportSizeLocation = glGetUniformLocation(m_program, "viewportSize");
    m_viewportOriginLocation = glGetUniformLocation(m_program, "viewportOrigin");
    m_curTimeLocation = glGetUniformLocation(m_program, "curTime");
    m_accelerationLocation = glGetUniformLocation(m_program, "acceleration");
    m_particleSizeLocation = glGetUniformLocation(m_program, "particleSize");
    m_useTexLocation = glGetUniformLocation(m_program, "useTex");
    m_textureLocation = glGetUniformLocation(m_program, "texture");
    m_positionLocation = glGetAttribLocation(m_program, "position");
    m_colorLocation = glGetAttribLocation(m_program, "color");
    m_directionLocation = glGetAttribLocation(m_program, "direction");
    m_initSpeedLocation = glGetAttribLocation(m_program, "initSpeed");
    m_startTimeLocation = glGetAttribLocation(m_program, "startTime");
}

void ParticleShaderProgram::setTexture(int textureId)
{
    glUniform1i(m_textureLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void ParticleShaderProgram::bindData(VertexArray& va)
{
    static const unsigned int COLOR_OFFSET = Constants::POSITION_SIZE;
    static const unsigned int DIRECTION_OFFSET = COLOR_OFFSET + Constants::COLOR_SIZE;
    static const unsigned int INIT_SPEED_OFFSET = DIRECTION_OFFSET + Constants::DIRECTION_SIZE;
    static const unsigned int START_TIME_OFFSET = INIT_SPEED_OFFSET + sizeof(float);

    glBindVertexArray(va.vao());
    glBindBuffer(GL_ARRAY_BUFFER, va.vbo());

    glVertexAttribPointer(m_positionLocation, Constants::NUM_FLOATS_PER_POSITION,
                          GL_FLOAT, GL_FALSE, va.stride(), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(m_positionLocation);

    glVertexAttribPointer(m_colorLocation, Constants::NUM_FLOATS_COLOR,
                          GL_FLOAT, GL_FALSE, va.stride(), reinterpret_cast<void*>(COLOR_OFFSET));
    glEnableVertexAttribArray(m_colorLocation);

    glVertexAttribPointer(m_directionLocation, Constants::NUM_FLOATS_DIRECTION,
                          GL_FLOAT, GL_FALSE, va.stride(), reinterpret_cast<void*>(DIRECTION_OFFSET));
    glEnableVertexAttribArray(m_directionLocation);

    glVertexAttribPointer(m_initSpeedLocation, 1,
                          GL_FLOAT, GL_FALSE, va.stride(), reinterpret_cast<void*>(INIT_SPEED_OFFSET));
    glEnableVertexAttribArray(m_initSpeedLocation);

    glVertexAttribPointer(m_startTimeLocation, 1,
                          GL_FLOAT, GL_FALSE, va.stride(), reinterpret_cast<void*>(START_TIME_OFFSET));
    glEnableVertexAttribArray(m_startTimeLocation);
}

} // end of namespace bot
