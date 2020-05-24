#ifndef INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE_PARSER
#define INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"

namespace bot {

class ParticleEffectTemplate;

class ParticleEffectTemplateParser {
public:
    ParticleEffectTemplateParser(const NamedMap<Texture>& textureLib, const NamedMap<Color>& colorLib)
        : m_textureLib(textureLib)
        , m_colorLib(colorLib)
    {}

    ~ParticleEffectTemplateParser()
    {}

    ParticleEffectTemplate* parse(const rapidjson::Value& elem);

private:
    const NamedMap<Texture>& m_textureLib;
    const NamedMap<Color>& m_colorLib;
};

} // end of namespace bot

#endif
