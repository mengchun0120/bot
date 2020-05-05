#ifndef INCLUDE_BOT_GAMEOBJECTITEM
#define INCLUDE_BOT_GAMEOBJECTITEM

#include "structure/bot_linkeditem.h"

namespace bot {

class GameObject;

class GameObjectItem : public LinkedItem {
public:
    GameObjectItem()
        : LinkedItem()
        , m_obj(nullptr)
    {}

    virtual ~GameObjectItem()
    {}

    GameObject* getObj()
    {
        return m_obj;
    }

    const GameObject* getObj() const
    {
        return m_obj;
    }

    void setObj(GameObject* obj)
    {
        m_obj = obj;
    }

protected:
    GameObject* m_obj;
};

} // end of namespace bot

#endif
