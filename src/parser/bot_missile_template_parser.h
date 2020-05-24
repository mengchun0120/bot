#ifndef INCLUDE_BOT_MISSILE_TEMPLATE_PARSER
#define INCLUDE_BOT_MISSILE_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_particle_effect_template.h"

namespace bot {

class MissileTemplate;

class MissileTemplateParser {
public:
	MissileTemplateParser(const NamedMap<Texture>& textureLib, 
					      const NamedMap<Rectangle>& rectLib,
						  const NamedMap<Color>& colorLib,
		                  const NamedMap<ParticleEffectTemplate>& particleEffectTemplateLib)
		: m_textureLib(textureLib)
		, m_rectLib(rectLib)
		, m_colorLib(colorLib)
		, m_particleEffectTemplateLib(particleEffectTemplateLib)
	{}

	~MissileTemplateParser()
	{}

    MissileTemplate* parse(const rapidjson::Value& elem);

private:
	const NamedMap<Texture>& m_textureLib;
	const NamedMap<Rectangle>& m_rectLib;
	const NamedMap<Color>& m_colorLib;
	const NamedMap<ParticleEffectTemplate>& m_particleEffectTemplateLib;
};

} // end of namespace bot

#endif
