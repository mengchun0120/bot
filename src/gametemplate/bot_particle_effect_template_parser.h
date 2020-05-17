#ifndef INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE_PARSER
#define INCLUDE_BOT_PARTICLE_EFFECT_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_lib.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"

namespace bot {

class ParticleEffectTemplate;

class ParticleEffectTemplateParser {
public:
    ParticleEffectTemplateParser(const NamedLib<Texture>& textureLib, const NamedLib<Color>& colorLib)
        : m_textureLib(textureLib)
        , m_colorLib(colorLib)
    {}

    ~ParticleEffectTemplateParser()
    {}

    bool parse(ParticleEffectTemplate* particleDataTemplate, const rapidjson::Value& elem);

private:
    const NamedLib<Texture>& m_textureLib;
    const NamedLib<Color>& m_colorLib;
};

} // end of namespace bot

#endif
