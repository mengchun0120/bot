#ifndef INCLUDE_BOT_TILE_TEMPLATE_PARSER
#define INCLUDE_BOT_TILE_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"

namespace bot {

class TileTemplate;

class TileTemplateParser {
public:
	TileTemplateParser(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
		               const NamedMap<Color>& colorLib)
		: m_textureLib(textureLib)
		, m_rectLib(rectLib)
		, m_colorLib(colorLib)
	{}

	~TileTemplateParser()
	{}

    TileTemplate* parse(const rapidjson::Value& elem);

private:
	const NamedMap<Texture>& m_textureLib;
	const NamedMap<Rectangle>& m_rectLib;
	const NamedMap<Color>& m_colorLib;
};

} // end of namespace bot

#endif
