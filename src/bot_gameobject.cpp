#include "bot_log.h"
#include "bot_app.h"
#include "bot_gameobject.h"

namespace bot {

GameObject::GameObject()
: m_template(nullptr)
, m_coverStartRow(-1)
, m_coverEndRow(-1)
, m_coverStartCol(-1)
, m_coverEndCol(-1)
{
}

GameObject::GameObject(GameObjectTemplate *t, float x, float y)
{
    init(t, x, y);
}

GameObject::~GameObject()
{
}

void GameObject::init(GameObjectTemplate *t, float x, float y)
{
    m_template = t;
    m_base.init(m_template->getBaseComponent(), x, y);
    
    int count = m_template->numParts();
    m_parts.resize(count);
    for(int i = 0; i < count; ++i) {
        GameObjectTemplate::Part &p = m_template->getPart(i);
        m_parts[i].init(p.m_component, p.m_pos[0] + x, p.m_pos[1] + y);
    }
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

} // end of namespace bot
