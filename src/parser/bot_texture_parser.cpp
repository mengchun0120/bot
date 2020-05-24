#include "misc/bot_log.h"
#include "misc/bot_file_utils.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_texture.h"
#include "parser/bot_texture_parser.h"

namespace bot {

Texture* TextureParser::parse(const rapidjson::Value& elem)
{
	std::string fileName;
	if (!parseJson(fileName, elem, "file")) 
	{
		LOG_ERROR("TextureLibParser: failed to parse file");
		return nullptr;
	}

	std::string filePath = constructPath({m_textureLibDir, fileName});
    
    Texture* texture = new Texture();
	if (!texture->load(filePath)) 
	{
		LOG_ERROR("TextureLibParser: failed to load texture from %s", filePath.c_str());
        delete texture;
		return nullptr;
	}

	return texture;
}

} // end of namespace bot
