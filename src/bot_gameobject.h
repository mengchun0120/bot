#ifndef INCLUDE_BOT_GAMEOBJECT
#define INCLUDE_BOT_GAMEOBJECT

#include <vector>
#include "bot_component.h"

namespace bot {

class ComponentTemplate;

class GameObject {
public:
    GameObject();

    GameObject(GameObjectTemplate *t, float x, float y);

    virtual ~GameObject();

    void init(GameObjectTemplate *t, float x, float y);

    int update(float delta);

    void present();

private:
    GameObjectTemplate *m_template;
    Component m_base;
    std::vector<Component> m_parts;
};

} // end of namespace bot

#endif

