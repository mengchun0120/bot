#ifndef INCLUDE_BOT_NAMEDLIB
#define INCLUDE_BOT_NAMEDLIB

#include <string>
#include <vector>
#include "misc/bot_log.h"
#include "misc/bot_jsonutils.h"
#include "structure/bot_nameidmap.h"

namespace bot {

template <typename T>
class NamedLib {
public:
	NamedLib()
	{}

	virtual ~NamedLib()
	{}

	int getLibSize() const
	{
		return static_cast<int>(m_lib.size());
	}

	void setLibSize(int size)
	{
		m_lib.resize(size);
	}

	const T* getObject(int idx) const
	{
		return &m_lib[idx];
	}

	T* getObject(int idx)
	{
		return &m_lib[idx];
	}

	const T* getObject(const char* name) const;

	T* getObject(const char* name);

	bool addNameIdxMapping(const char* name, int idx);

protected:
	std::vector<T> m_lib;
	NameIdMap m_nameIdxMap;
};

template <typename T>
const T* NamedLib<T>::getObject(const char* name) const
{
	int idx = m_nameIdxMap.search(name);
	return idx >= 0 ? &m_lib[idx] : nullptr;
}

template <typename T>
T* NamedLib<T>::getObject(const char* name)
{
	int idx = m_nameIdxMap.search(name);
	return idx >= 0 ? &m_lib[idx] : nullptr;
}

template <typename T>
bool NamedLib<T>::addNameIdxMapping(const char* name, int idx)
{
	return m_nameIdxMap.add(name, idx);
}

template <typename T, typename PARSER>
bool readNamedLibFromJson(NamedLib<T>& lib, const char* fileName, PARSER& parser)
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

	if (numObjects == 0)
	{
		return true;
	}

	lib.setLibSize(numObjects);
	for (int i = 0; i < numObjects; ++i) 
	{
		const rapidjson::Value& elem = arr[i];
		std::string name;

		if (!parseJson(name, elem, "name")) 
		{
			return false;
		}

		if (!lib.addNameIdxMapping(name.c_str(), i))
		{
			LOG_ERROR("Error in the %dth object in %s: name %s is not unique", i, fileName, name.c_str());
			return false;
		}

		T* obj = lib.getObjectByIdx(i);
		if (!parser.parse(obj, elem))
		{
			LOG_ERROR("Error in the %dth object in %s: failed to parse object", i, fileName);
			return false;
		}
	}

	return true;
}

} // end of namespace bot

#endif
