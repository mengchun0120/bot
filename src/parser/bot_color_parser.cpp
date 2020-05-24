#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "opengl/bot_color.h"
#include "parser/bot_color_parser.h"

namespace bot {

Color* ColorParser::parse(const rapidjson::Value& elem)
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
		return nullptr;
	}

    Color* color = new Color();
	if (!color->setColor(red, green, blue, alpha)) 
	{
		LOG_ERROR("Failed to set color");
        delete color;
		return nullptr;
	}

	return color;
}

} // end of namespace bot
