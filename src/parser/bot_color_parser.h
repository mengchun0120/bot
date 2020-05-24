#ifndef INCLUDE_BOT_COLOR_PARSER
#define INCLUDE_BOT_COLOR_PARSER

#include <rapidjson/document.h>

namespace bot {

class Color;

class ColorParser {
public:
	ColorParser()
	{}

	~ColorParser()
	{}

    Color* parse(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
