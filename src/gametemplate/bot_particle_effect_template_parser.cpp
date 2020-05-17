#include "misc/bot_json_utils.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "gametemplate/bot_particle_effect_template_parser.h"

namespace bot {

bool ParticleEffectTemplateParser::parse(ParticleEffectTemplate* particleEffectTemplate, const rapidjson::Value& elem)
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
        return false;
    }

    if (data.size() == 0)
    {
        LOG_ERROR("Particles must NOT be empty");
        return false;
    }

    const int NUM_FLOATS_PER_PARTICLE = Constants::NUM_FLOATS_PER_POSITION;

    if (data.size() % NUM_FLOATS_PER_PARTICLE != 0)
    {
        LOG_ERROR("Length of particles must be multiples of %d", NUM_FLOATS_PER_PARTICLE);
        return false;
    }

    int numParticles = data.size() / NUM_FLOATS_PER_PARTICLE;

    const Texture* texture = m_textureLib.getObject(textureName.c_str());
    if (!texture)
    {
        LOG_ERROR("Cannot find texture %s", textureName.c_str());
        return false;
    }

    const Color* color = m_colorLib.getObject(colorName.c_str());
    if (!color)
    {
        LOG_ERROR("Cannot find color %s", colorName.c_str());
        return false;
    }

    bool success = particleEffectTemplate->init(coverBreathX, coverBreathY, numParticles, acceleration, initSpeed,
                                                duration, particleSize, data.data(), texture, color);
    if (!success)
    {
        LOG_ERROR("Failed to initilize particle effect template");
        return false;
    }

    return true;
}

} // end of namespace bot
