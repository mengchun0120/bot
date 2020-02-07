#include "bot_log.h"
#include "bot_app.h"
#include "bot_utils.h"
#include "bot_gameobject.h"

namespace bot {

GameObject* GameObject::createFromTemplate(const GameObjectTemplate* t)
{
    GameObject* obj = new GameObject();

    obj->m_template = t;
    obj->m_hp = t->getHP();

    obj->m_base.init(t->getBaseComponent(), 0.0f, 0.0f);

    int numParts = t->numParts();

    obj->m_parts.resize(numParts);
    for (int i = 0; i < numParts; ++i) {
        const GameObjectTemplate::Part& p = t->getPart(i);
        obj->m_parts[i].init(p.m_component, p.m_pos[0], p.m_pos[1]);
    }

    return obj;
}

GameObject::GameObject()
    : DoubleLinkedItem()
    , m_template(nullptr)
    , m_coverStartRow(-1)
    , m_coverEndRow(-1)
    , m_coverStartCol(-1)
    , m_coverEndCol(-1)
    , m_flags(0)
    , m_hp(0)
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
