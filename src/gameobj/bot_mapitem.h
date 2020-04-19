#ifndef INCLUDE_BOT_MAPITEM
#define INCLUDE_BOT_MAPITEM

#include "structure/bot_linkeditem.h"

namespace bot {

class GameObject;

class MapItem : public LinkedItem {
public:
    MapItem()
        : LinkedItem()
        , m_obj(nullptr)
    {}

    virtual ~MapItem()
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
