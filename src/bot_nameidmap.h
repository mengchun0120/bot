#ifndef INCLUDE_BOT_NAMEIDMAP
#define INCLUDE_BOT_NAMEIDMAP

#include <string>

namespace bot {

class NameIdMap {
public:
    NameIdMap();

    virtual ~NameIdMap();

    bool add(const char *name, int idx);

    bool add(const std::string &name, int idx)
    {
        return add(name.c_str(), idx);
    }

    int search(const char *name) const;

    int search(const std::string &name) const
    {
        return search(name.c_str());
    }

    void clear();

    void print();

private:
    struct Node {
        Node *m_left, *m_right;
        char m_ch;
        int m_idx;

        Node(char ch)
        : m_left(nullptr)
        , m_right(nullptr)
        , m_ch(ch)
        , m_idx(-1)
        {}
    };

    Node *m_root;
};

} // end of namespace bot

#endif
