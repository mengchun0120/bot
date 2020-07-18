#ifndef INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE
#define INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE

#include <rapidjson/document.h>
#include "opengl/bot_vertex_array.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

class Texture;
class Color;

class ParticleEffectTemplate: public GameObjectTemplate {
public:
    static ParticleEffectTemplate* create(const rapidjson::Value& elem);
    
    ParticleEffectTemplate();

    virtual ~ParticleEffectTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    bool init(float coverBreathX, float coverBreathY, int numParticles, float accelearation, float initSpeed,
              float duration, float particleSize, const float* data, const Texture* texture, const Color* color);

    int getNumParticles() const
    {
        return m_numParticles;
    }

    float getAcceleration() const
    {
        return m_acceleration;
    }

    float getInitSpeed() const
    {
        return m_initSpeed;
    }

    float getDuration() const
    {
        return m_duration;
    }

    float getParticleSize() const
    {
        return m_particleSize;
    }

    const VertexArray& getVertexArray() const
    {
        return m_vertexArray;
    }

    const Texture* getTexture() const
    {
        return m_texture;
    }

    const Color* getColor() const
    {
        return m_color;
    }

protected:
    int m_numParticles;
    float m_acceleration;
    float m_initSpeed;
    float m_duration;
    float m_particleSize;
    VertexArray m_vertexArray;
    const Texture* m_texture;
    const Color* m_color;
};

} // end of namespace bot

#endif
