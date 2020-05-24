#ifndef INCLUDE_BOT_RECTANGLE_PARSER
#define INCLUDE_BOT_RECTANGLE_PARSER

#include <rapidjson/document.h>

namespace bot {

class Rectangle;

class RectangleParser {
public:
	RectangleParser()
	{}

	~RectangleParser()
	{}

    Rectangle* parse(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
