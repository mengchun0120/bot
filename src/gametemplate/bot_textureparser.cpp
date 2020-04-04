#include "misc/bot_log.h"
#include "misc/bot_fileutils.h"
#include "misc/bot_jsonutils.h"
#include "opengl/bot_texture.h"
#include "gametemplate/bot_textureparser.h"

namespace bot {

bool TextureParser::parse(Texture* texture, const rapidjson::Value& elem)
{
	std::string fileName;
	if (!parseJson(fileName, elem, "file")) 
	{
		LOG_ERROR("TextureLibParser: failed to parse file");
		return false;
	}

	std::string filePath = constructPath({m_textureLibDir, fileName});
	if (!texture->load(filePath)) 
	{
		LOG_ERROR("TextureLibParser: failed to load texture from %s", filePath.c_str());
		return false;
	}

	return true;
}

} // end of namespace bot
