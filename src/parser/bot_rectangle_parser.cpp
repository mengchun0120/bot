#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "geometry/bot_rectangle.h"
#include "parser/bot_rectangle_parser.h"

namespace bot {

Rectangle* RectangleParser::parse(const rapidjson::Value& elem)
{
	float width = 0.0f, height = 0.0f;
	std::vector<JsonParseParam> params = 
	{
		{&width, "width", JSONTYPE_FLOAT},
		{&height, "height", JSONTYPE_FLOAT}
	};

	if (!parseJson(params, elem)) 
	{
		return nullptr;
	}

    Rectangle* rect = new Rectangle();

	if (!rect->init(width, height, true)) 
	{
		LOG_ERROR("Failed to initialize rectangle");
        delete rect;
		return nullptr;
	}

	return rect;
}

} // end of namespace bot
