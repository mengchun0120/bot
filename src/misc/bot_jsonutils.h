#ifndef INCLUDE_BOT_JSONUTILS
#define INCLUDE_BOT_JSONUTILS

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_jsondatatype.h"

namespace bot {

struct JsonParseParam {
	void* m_ptr;
	const char* m_name;
	JsonDataType m_type;
};

bool readJson(rapidjson::Document& doc, const char* fileName);

int validateJson(const rapidjson::Value& value, const char* name, JsonDataType type);

bool parseJson(int& r, const rapidjson::Value& value, const char* name);

bool parseJson(float& r, const rapidjson::Value& value, const char* name);

bool parseJson(bool& r, const rapidjson::Value& value, const char* name);

bool parseJson(double& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::string& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<int>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<float>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<double>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<bool>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<std::string>& r, const rapidjson::Value& value, const char* name);

bool parseJson(std::vector<JsonParseParam>& params, const rapidjson::Value& value);

} // end of namespace bot

#endif
