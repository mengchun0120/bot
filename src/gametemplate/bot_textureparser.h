#ifndef INCLUDE_BOT_TEXTURELIBPARSER
#define INCLUDE_BOT_TEXTURELIBPARSER

#include <rapidjson/document.h>
#include <string>

namespace bot {

class Texture;

class TextureParser {
public:
	TextureParser(const std::string& textureLibDir)
		: m_textureLibDir(textureLibDir)
	{}

	~TextureParser()
	{}

	bool parse(Texture* texture, const rapidjson::Value& elem);

private:
	std::string m_textureLibDir;
};

} // end of namespace bot

#endif
