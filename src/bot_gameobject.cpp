#include <rapidjson/document.h>
#include "bot_log.h"
#include "bot_app.h"
#include "bot_utils.h"
#include "bot_gameobject.h"

using namespace rapidjson;

namespace bot {

bool validateGameObjectDoc(const Document& doc)
{
    if (!doc.HasMember("baseComponent")) {
        LOG_ERROR("baseComponent missing");
        return false;
    }

    if (!doc["baseComponent"].IsString()) {
        LOG_ERROR("Invalid baseComponent");
        return false;
    }

    if (!doc.HasMember("coverBreath")) {
        LOG_ERROR("coverBreath missing");
        return false;
    }

    const Value& breath = doc["coverBreath"];
    if (!breath.IsArray() || breath.Size() != 2 || !breath[0].IsFloat() || !breath[1].IsFloat()) {
        LOG_ERROR("Invalid coverBreath");
        return false;
    }

    if (!doc.HasMember("parts")) {
        LOG_ERROR("parts missing");
        return false;
    }

    const Value& parts = doc["parts"];
    if (!parts.IsArray()) {
        LOG_ERROR("Invalid parts");
        return false;
    }

    for (SizeType i = 0; i < parts.Size(); ++i) {
        const Value& p = parts[i];
        if (!p.IsArray() || p.Size() != 3 || !p[0].IsString() || !p[1].IsFloat() || !p[2].IsFloat()) {
            LOG_ERROR("Invalid part at %d", i);
            return false;
        }
    }

    return true;
}

GameObject* GameObject::createFromTemplate(const GameObjectTemplate* t)
{
    GameObject* obj = new GameObject();

    obj->m_coverBreathX = t->getCoverBreathX();
    obj->m_coverBreathY = t->getCoverBreathY();

    obj->m_base.init(t->getBaseComponent(), 0.0f, 0.0f);

    int numParts = t->numParts();

    obj->m_parts.resize(numParts);
    for (int i = 0; i < numParts; ++i) {
        const GameObjectTemplate::Part& p = t->getPart(i);
        obj->m_parts[i].init(p.m_component, p.m_pos[0], p.m_pos[1]);
    }

    return obj;
}

GameObject* GameObject::createFromJson(const char* jsonFile)
{
    Document doc;
    if (!readJson(doc, jsonFile)) {
        return nullptr;
    }

    if (!validateGameObjectDoc(doc)) {
        return nullptr;
    }

    GameObject* obj = new GameObject();

    const Value& coverBreath = doc["coverBreath"];
    obj->m_coverBreathX = coverBreath[0].GetFloat();
    obj->m_coverBreathY = coverBreath[1].GetFloat();

    const GameLib& lib = App::g_app.gameLib();
    const char* baseName = doc["baseComponent"].GetString();
    const ComponentTemplate* baseTemplate = lib.getComponentTemplate(baseName);
    if (!baseTemplate) {
        LOG_ERROR("Failed to find baseComponent %s", baseName);
        delete obj;
        return nullptr;
    }

    obj->m_base.init(baseTemplate, 0.0f, 0.0f);

    const Value& parts = doc["parts"];
    int numParts = static_cast<int>(parts.Size());

    obj->m_parts.resize(numParts);
    for (int i = 0; i < numParts; ++i) {
        const Value& p = parts[i];
        const char* partName = p[0].GetString();
        const ComponentTemplate* partTemplate = lib.getComponentTemplate(partName);
        if (!partTemplate) {
            LOG_ERROR("Failed to find component %s", partName);
            delete obj;
            return nullptr;
        }

        float x = p[1].GetFloat();
        float y = p[2].GetFloat();
        obj->m_parts[i].init(partTemplate, x, y);
    }

    return obj;
}

GameObject::GameObject()
    : DoubleLinkedItem()
    , m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_coverStartRow(-1)
    , m_coverEndRow(-1)
    , m_coverStartCol(-1)
    , m_coverEndCol(-1)
    , m_flags(0)
{
}

GameObject::~GameObject()
{
}

int GameObject::update(float delta)
{
    return 0;
}

void GameObject::present()
{
    m_base.present();

    int count = static_cast<int>(m_parts.size());
    for(int i = 0; i < count; ++i) {
        m_parts[i].present();
    }
}

void GameObject::setPos(float x, float y)
{
    float deltaX = x - m_base.getX();
    float deltaY = y - m_base.getY();
    move(deltaX, deltaY);
}

void GameObject::move(float deltaX, float deltaY)
{
    m_base.move(deltaX, deltaY);

    int count = static_cast<int>(m_parts.size());
    for (int i = 0; i < count; ++i) {
        m_parts[i].move(deltaX, deltaY);
    }
}

void GameObject::setDirection(float directionX, float directionY)
{
    m_base.setDirection(directionX, directionY);

    int count = static_cast<int>(m_parts.size());
    for (int i = 0; i < count; ++i) {
        m_parts[i].setDirection(directionX, directionY);
    }
}

} // end of namespace bot
