#include "misc/bot_json_utils.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "parser/bot_particle_effect_template_parser.h"

namespace bot {

ParticleEffectTemplate* ParticleEffectTemplateParser::parse(const rapidjson::Value& elem)
{
    float coverBreathX, coverBreathY;
    float acceleration, initSpeed, duration, particleSize;
    std::vector<float> data;
    std::string textureName, colorName;

    std::vector<JsonParseParam> params =
    {
        {&coverBreathX, "coverBreathX", JSONTYPE_FLOAT},
        {&coverBreathY, "coverBreathY", JSONTYPE_FLOAT},
        {&acceleration, "acceleration", JSONTYPE_FLOAT},
        {&initSpeed,    "initSpeed",    JSONTYPE_FLOAT},
        {&duration,     "duration",     JSONTYPE_FLOAT},
        {&particleSize, "particleSize", JSONTYPE_FLOAT},
        {&textureName,  "texture",      JSONTYPE_STRING},
        {&colorName,    "color",        JSONTYPE_STRING},
        {&data,         "particles",    JSONTYPE_FLOAT_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return nullptr;
    }

    if (data.size() == 0)
    {
        LOG_ERROR("Particles must NOT be empty");
        return nullptr;
    }

    const int NUM_FLOATS_PER_PARTICLE = Constants::NUM_FLOATS_PER_POSITION;

    if (data.size() % NUM_FLOATS_PER_PARTICLE != 0)
    {
        LOG_ERROR("Length of particles must be multiples of %d", NUM_FLOATS_PER_PARTICLE);
        return nullptr;
    }

    int numParticles = data.size() / NUM_FLOATS_PER_PARTICLE;

    const Texture* texture = m_textureLib.search(textureName);
    if (!texture)
    {
        LOG_ERROR("Cannot find texture %s", textureName.c_str());
        return nullptr;
    }

    const Color* color = m_colorLib.search(colorName);
    if (!color)
    {
        LOG_ERROR("Cannot find color %s", colorName.c_str());
        return nullptr;
    }

    ParticleEffectTemplate* particleEffectTemplate = new ParticleEffectTemplate();

    bool success = particleEffectTemplate->init(coverBreathX, coverBreathY, numParticles, acceleration, initSpeed,
                                                duration, particleSize, data.data(), texture, color);
    if (!success)
    {
        LOG_ERROR("Failed to initilize particle effect template");
        delete particleEffectTemplate;
        return nullptr;
    }

    return particleEffectTemplate;
}

} // end of namespace bot
