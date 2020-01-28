#ifndef INCLUDE_BOT_LINKEDITEM
#define INCLUDE_BOT_LINKEDITEM

namespace bot {

class LinkedItem {
public:
    LinkedItem()
    : m_next(nullptr)
    {}

    virtual ~LinkedItem()
    {}

    const LinkedItem* getNext() const
    {
        return m_next;
    }

    LinkedItem* getNext()
    {
        return m_next;
    }

    void setNext(LinkedItem *next)
    {
        m_next = next;
    }

protected:
    LinkedItem *m_next;
};

} // end of namespace bot

#endif
