#ifndef INCLUDE_BOT_DOUBLELINKITEM
#define INCLUDE_BOT_DOUBLELINKITEM

#include "structure/bot_linkeditem.h"

namespace bot {

class DoubleLinkedItem : public LinkedItem {
public:
    DoubleLinkedItem()
        : m_prev(nullptr)
    {}

    virtual ~DoubleLinkedItem()
    {}

    const LinkedItem* getPrev() const
    {
        return m_prev;
    }

    LinkedItem* getPrev()
    {
        return m_prev;
    }

    void setPrev(LinkedItem* prev)
    {
        m_prev = prev;
    }

protected:
    LinkedItem* m_prev;
};

} // end of namespace bot

#endif
