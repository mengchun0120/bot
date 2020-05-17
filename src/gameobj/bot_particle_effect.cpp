#include "misc/bot_log.h"
#include "opengl/bot_particle_shader_program.h"
#include "opengl/bot_color.h"
#include "opengl/bot_texture.h"
#include "screen/bot_game_screen.h"
#include "gameobj/bot_particle_effect.h"

namespace bot {

ParticleEffect::ParticleEffect()
    : GameObject(nullptr)
    , m_startTime(Clock::now())
    , m_duration(0.0f)
{
}

ParticleEffect::ParticleEffect(const ParticleEffectTemplate* t)
    : GameObject(t)
    , m_startTime(Clock::now())
    , m_duration(0.0f)
{

}

void ParticleEffect::init(const ParticleEffectTemplate* t, float x, float y)
{
    m_template = t;
    m_pos[0] = x;
    m_pos[1] = y;
    m_startTime = Clock::now();
    m_duration = 0.0f;
}

void ParticleEffect::present(ShaderProgram& program)
{
    ParticleShaderProgram& particleProgram = static_cast<ParticleShaderProgram&>(program);
    const ParticleEffectTemplate* t = getTemplate();

    particleProgram.setRef(m_pos);
    particleProgram.setAcceleration(t->getAcceleration());
    particleProgram.setInitSpeed(t->getInitSpeed());
    particleProgram.setColor(t->getColor()->getColor());
    particleProgram.setParticleSize(t->getParticleSize());
    particleProgram.setCurTime(m_duration);
    particleProgram.setUseTex(true);
    particleProgram.setTexture(t->getTexture()->textureId());
    particleProgram.bindData(t->getVertexArray());

    glDrawArrays(GL_POINTS, 0, t->getNumParticles());
}

bool ParticleEffect::update(float delta, GameScreen& screen)
{
    GameObjectManager& gameObjManager = screen.getGameObjManager();

    if (screen.getMap().isOutsideViewport(this)) {
        gameObjManager.sendToDeathQueue(this);
        return false;
    }

    if (elapsedTimeMs(m_startTime) >= getTemplate()->getDuration())
    {
        gameObjManager.sendToDeathQueue(this);
        return false;
    }

    return true;
}

} // end of namespace bot
