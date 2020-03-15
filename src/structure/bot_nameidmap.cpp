#include <list>
#include <iostream>
#include "misc/bot_log.h"
#include "structure/bot_nameidmap.h"

namespace bot {

NameIdMap::NameIdMap()
: m_root(nullptr)
{}

NameIdMap::~NameIdMap()
{
    clear();
}

bool NameIdMap::add(const char *name, int idx)
{
    if(idx < 0) {
        LOG_ERROR("Invalid idx %d", idx);
        return false;
    }

    const char *ch;
    Node *parent = nullptr;
    Node *n = m_root;

    for(ch = name; *ch != '\0' && n; ++ch) {
        Node *prev = nullptr;
        Node *m = n;

        while(m && m->m_ch < *ch) {
            prev = m;
            m = m->m_right;
        }

        if(m && m->m_ch == *ch) {
            parent = m;
            n = m->m_left;
        } else {
            Node *p = new Node(*ch);

            p->m_right = m;
            if(prev) {
                prev->m_right = p;
            } else if(parent) {
                parent->m_left = p;
            } else {
                m_root = p;
            }

            parent = p;
            n = nullptr;
        }
    }

    if(n) {
        if(n->m_idx != idx) {
            // a name can only map to one idx
            LOG_WARN("Trying to add a different idx %d for name %s", idx, name);
            return false;
        } else {
            // alreay in the map
            return true;
        }
    }

    for(; *ch != '\0'; ++ch) {
        n = new Node(*ch);
        if(parent) {
            parent->m_left = n;
        } else {
            m_root = n;
        }
        parent = n;
    }

    if(parent) {
        parent->m_idx = idx;
    }

    return true;
}


int NameIdMap::search(const char *name) const
{
    Node *n = m_root;
    const char *ch = name;

    while(*ch != '\0' && n) {
        while(n && n->m_ch < *ch) {
            n = n->m_right;
        }

        if(!n || n->m_ch != *ch) {
            return -1;
        }

        ++ch;
        if(*ch != '\0') {
            n = n->m_left;
        }
    }

    return n ? n->m_idx : -1;
}

void NameIdMap::clear()
{
    if(!m_root) {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while(!q.empty()) {
        Node *n = q.front();
        q.pop_front();

        if(n->m_left) {
            q.push_back(n->m_left);
        }

        if(n->m_right) {
            q.push_back(n->m_right);
        }

        delete n;
    }
}

void NameIdMap::print()
{
    if(!m_root) {
        return;
    }

    std::list<Node*> q;
    q.push_back(m_root);

    while(!q.empty()) {
        Node *n = q.front();
        q.pop_front();

        std::cout << n->m_ch << ' ' << n->m_idx;

        if(n->m_left) {
            std::cout << ' ' << n->m_left->m_ch;
        } else {
            std::cout << " null";
        }

        if(n->m_right) {
            std::cout << ' ' << n->m_right->m_ch;
        } else {
            std::cout << " null";
        }

        std::cout << std::endl;

        if(n->m_left) {
            q.push_back(n->m_left);
        }

        if(n->m_right) {
            q.push_back(n->m_right);
        }
    }
}

} // end of namespace bot
