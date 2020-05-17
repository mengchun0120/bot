#include "misc/bot_constants.h"
#include "gametemplate/bot_particle_effect_template.h"

namespace bot {

ParticleEffectTemplate::ParticleEffectTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_PARTICLE_EFFECT)
    , m_numParticles(0)
    , m_acceleration(0.0f)
    , m_initSpeed(0.0f)
    , m_duration(0.0f)
    , m_texture(nullptr)
    , m_color(nullptr)
{}

bool ParticleEffectTemplate::init(float coverBreathX, float coverBreathY, int numParticles, float accelearation, 
                                  float initSpeed, float duration, float particleSize, const float* data, 
                                  const Texture* texture, const Color* color)
{
    const int VERTEX_SIZE = Constants::POSITION_SIZE;

    if (!m_vertexArray.load(data, numParticles, VERTEX_SIZE, VERTEX_SIZE))
    {
        return false;
    }

    setCoverBreathX(coverBreathX);
    setCoverBreathY(coverBreathY);

    m_numParticles = numParticles;
    m_acceleration = accelearation;
    m_initSpeed = initSpeed;
    m_duration = duration;
    m_particleSize = particleSize;
    m_texture = texture;
    m_color = color;

    return true;
}

} // end of namespace bot
