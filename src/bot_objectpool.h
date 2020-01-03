#ifndef INCLUDE_BOT_OBJECTPOOL
#define INCLUDE_BOT_OBJECTPOOL

#include <vector>
#include <type_traits>
#include "bot_linkeditem.h"

namespace bot {

template <typename T>
class ObjectPool {
public:
    static_assert(std::is_base_of<LinkedItem, T>::value,
                  "T must be derived from LinkedItem");

    ObjectPool()
    : m_free(nullptr)
    , m_pool(nullptr)
    {}

    ObjectPool(int capacity)
    {
        init(capacity);
    }

    virtual ~ObjectPool()
    {
        delete[] m_pool;
    }

    void init(int capacity)
    {
        m_capacity = capacity;
        m_pool = new T[capacity];
        for(int i = 1; i < capacity; ++i) {
            m_pool[i-1].setNext(&m_pool[i]);
        }
        m_free = &m_pool[0];
    }

    T *alloc()
    {
        T *ret = nullptr;
        if(m_free) {
            T *ret = m_free;
            m_free = static_cast<T*>(m_free->getNext());
        } else {
            ret = new T();
        }
        return ret;
    }

    void free(T *t)
    {
        if(t >= m_free && t < m_free + m_capacity) {
            t->setNext(m_free);
            m_free = t;
        } else {
            delete t;
        }
    }

protected:
    int m_capacity;
    T *m_free;
    T *m_pool;
};

} // end of namespace bot

#endif
