#ifndef INCLUDE_BOT_NAMED_MAP
#define INCLUDE_BOT_NAMED_MAP

#include <string>
#include <list>
#include <iostream>
#include "misc/bot_log.h"

namespace bot {

template <typename T>
class NamedMap {
public:
    NamedMap();

    virtual ~NamedMap();

    bool add(const char *name, T* t);

    bool add(const std::string &name, T* t)
    {
        return add(name.c_str(), t);
    }

    T* search(const char *name) const;

    T* search(const std::string &name) const
    {
        return search(name.c_str());
    }

    void clear();

    void print();

private:
    struct Node {
        Node *m_left, *m_right;
        char m_ch;
        T* m_ptr;

        Node(char ch)
        : m_left(nullptr)
        , m_right(nullptr)
        , m_ch(ch)
        , m_ptr(nullptr)
        {}
    };

    Node *m_root;
};

template <typename T>
NamedMap<T>::NamedMap()
    : m_root(nullptr)
{}

template <typename T>
NamedMap<T>::~NamedMap()
{
    clear();
}

template <typename T>
bool NamedMap<T>::add(const char* name, T* t)
{
    const char* ch;
    Node* parent = nullptr;
    Node* n = m_root;

    for (ch = name; *ch != '\0' && n; ++ch) 
    {
        Node* prev = nullptr;
        Node* m = n;

        while (m && m->m_ch < *ch) 
        {
            prev = m;
            m = m->m_right;
        }

        if (m && m->m_ch == *ch) 
        {
            parent = m;
            n = m->m_left;
        }
        else
        {
            Node* p = new Node(*ch);

            p->m_right = m;
            if (prev)
            {
                prev->m_right = p;
            }
            else if (parent) 
            {
                parent->m_left = p;
            }
            else
            {
                m_root = p;
            }

            parent = p;
            n = nullptr;
        }
    }

    if (n) 
    {
        if (n->m_ptr != t) 
        {
            // a name can only map to one pointer
            LOG_WARN("Trying to add a different pointer for name %s", name);
            return false;
        }
        else
        {
            // alreay in the map
            return true;
        }
    }

    for (; *ch != '\0'; ++ch) 
    {
        n = new Node(*ch);
        if (parent) 
        {
            parent->m_left = n;
        }
        else
        {
            m_root = n;
        }
        parent = n;
    }

    parent->m_ptr = t;

    return true;
}

template <typename T>
T* NamedMap<T>::search(const char* name) const
{
    Node* n = m_root;
    const char* ch = name;

    while (*ch != '\0' && n) 
    {
        while (n && n->m_ch < *ch) 
        {
            n = n->m_right;
        }

        if (!n || n->m_ch != *ch) 
        {
            return nullptr;
        }

        ++ch;
        if (*ch != '\0') 
        {
            n = n->m_left;
        }
    }

    return n ? n->m_ptr : nullptr;
}

template <typename T>
void NamedMap<T>::clear()
{
    if (!m_root) 
    {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while (!q.empty()) 
    {
        Node* n = q.front();
        q.pop_front();

        if (n->m_left) 
        {
            q.push_back(n->m_left);
        }

        if (n->m_right) 
        {
            q.push_back(n->m_right);
        }

        if (n->m_ptr)
        {
            delete n->m_ptr;
        }

        delete n;
    }
}

template <typename T>
void NamedMap<T>::print()
{
    if (!m_root) 
    {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while (!q.empty()) 
    {
        Node* n = q.front();
        q.pop_front();

        std::cout << n->m_ch << ' ' << static_cast<void*>(n->m_ptr);

        if (n->m_left) 
        {
            std::cout << ' ' << n->m_left->m_ch;
        }
        else {
            std::cout << " null";
        }

        if (n->m_right)
        {
            std::cout << ' ' << n->m_right->m_ch;
        }
        else
        {
            std::cout << " null";
        }

        std::cout << std::endl;

        if (n->m_left) 
        {
            q.push_back(n->m_left);
        }

        if (n->m_right) 
        {
            q.push_back(n->m_right);
        }
    }
}

} // end of namespace bot

#endif
