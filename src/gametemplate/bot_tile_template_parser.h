#ifndef INCLUDE_BOT_TILE_TEMPLATE_PARSER
#define INCLUDE_BOT_TILE_TEMPLATE_PARSER

#include <rapidjson/document.h>
#include "structure/bot_named_lib.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"

namespace bot {

class TileTemplate;

class TileTemplateParser {
public:
	TileTemplateParser(const NamedLib<Texture>& textureLib, const NamedLib<Rectangle>& rectLib,
		               const NamedLib<Color>& colorLib)
		: m_textureLib(textureLib)
		, m_rectLib(rectLib)
		, m_colorLib(colorLib)
	{}

	~TileTemplateParser()
	{}

	bool parse(TileTemplate* tileTemplate, const rapidjson::Value& elem);

private:
	const NamedLib<Texture>& m_textureLib;
	const NamedLib<Rectangle>& m_rectLib;
	const NamedLib<Color>& m_colorLib;
};

} // end of namespace bot

#endif
