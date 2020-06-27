#ifndef INCLUDE_BOT_JSON_UTILS
#define INCLUDE_BOT_JSON_UTILS

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "misc/bot_log.h"
#include "misc/bot_json_data_type.h"
#include "structure/bot_named_map.h"

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

template <typename PROCESSOR>
bool parseJsonArray(const rapidjson::Value& value, PROCESSOR& processor, const char* name)
{
	if (!value.HasMember(name))
	{
		return false;
	}

	const rapidjson::Value& arr = value[name];
	if (!arr.IsArray())
	{
		return false;
	}

	int len = arr.Capacity();
	for (int i = 0; i < len; ++i)
	{
		if (!processor(arr[i]))
		{
			return false;
		}
	}

	return true;
}

template <typename T, typename PARSER>
bool parseNamedMap(NamedMap<T>& lib, const char* fileName, PARSER& parser)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName))
    {
        return false;
    }

    if (!doc.IsArray())
    {
        LOG_ERROR("Invalid format: %s", fileName);
        return false;
    }

    const rapidjson::Value& arr = doc.GetArray();
    int numObjects = arr.Capacity();

    for (int i = 0; i < numObjects; ++i)
    {
        const rapidjson::Value& elem = arr[i];
        std::string name;

        if (!parseJson(name, elem, "name"))
        {
            LOG_ERROR("Failed to find name in the %dth object of %s", i, fileName);
            return false;
        }

        T* t = parser.parse(elem);
        if (!t)
        {
            LOG_ERROR("Failed to parse the %dth object of %s", i, fileName);
            return false;
        }

        lib.add(name, t);
    }

    return true;
}

template <typename T, typename PARSER>
bool parseVector(std::vector<T>& vec, const char* file, PARSER& parser)
{
    rapidjson::Document doc;

    if (!readJson(doc, file))
    {
        return false;
    }

    if (!doc.IsArray())
    {
        LOG_ERROR("Invalid format: %s", file);
        return false;
    }

    const rapidjson::Value& arr = doc.GetArray();
    int numObjects = arr.Capacity();

    vec.resize(numObjects);
    for (int i = 0; i < numObjects; ++i)
    {
        const rapidjson::Value& elem = arr[i];
        if (!parser.parse(vec[i], elem))
        {
            LOG_ERROR("Failed to parse the %dth object of %s", i, file);
            return false;
        }
    }

    return true;
}

} // end of namespace bot

#endif
