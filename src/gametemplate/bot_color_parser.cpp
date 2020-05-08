#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_color_parser.h"

namespace bot {

bool ColorParser::parse(Color* color, const rapidjson::Value& elem)
{
	int red = 0, green = 0, blue = 0, alpha = 0;
	std::vector<JsonParseParam> params =
	{
		{&red,   "red",   JSONTYPE_INT},
		{&green, "green", JSONTYPE_INT},
		{&blue,  "blue",  JSONTYPE_INT},
		{&alpha, "alpha", JSONTYPE_INT}
	};

	if (!parseJson(params, elem)) 
	{
		return false;
	}

	if (!color->setColor(red, green, blue, alpha)) 
	{
		LOG_ERROR("Failed to set color");
		return false;
	}

	return true;
}

} // end of namespace bot
