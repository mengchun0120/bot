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
