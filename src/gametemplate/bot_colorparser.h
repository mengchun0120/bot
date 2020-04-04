#ifndef INCLUDE_BOT_COLORLIBPARSER
#define INCLUDE_BOT_COLORLIBPARSER

#include <rapidjson/document.h>

namespace bot {

class Color;

class ColorParser {
public:
	ColorParser()
	{}

	~ColorParser()
	{}

	bool parse(Color* color, const rapidjson::Value& elem);
};

} // end of namespace bot

#endif
