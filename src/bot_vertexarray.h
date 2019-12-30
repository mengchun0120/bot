#ifndef INCLUDE_BOT_VERTEXARRAY
#define INCLUDE_BOT_VERTEXARRAY

namespace bot {

class VertexArray {
public:
    VertexArray();

    VertexArray(const float* vertices, unsigned int numVertices0,
                bool hasTexCoord0);

    virtual ~VertexArray();

    bool load(const float* vertices, unsigned int numVertices0,
              bool hasTexCoord0);

    unsigned int numVertices() const
    {
        return m_numVertices;
    }

    bool hasTexCoord() const
    {
        return m_hasTexCoord;
    }

    unsigned int vertexSize() const
    {
        return m_vertexSize;
    }

    unsigned int stride() const
    {
        return m_stride;
    }

    unsigned int vao() const
    {
        return m_vao;
    }

    unsigned int vbo() const
    {
        return m_vbo;
    }

protected:
    void destroy();

    unsigned int m_numVertices;
    bool m_hasTexCoord;
    unsigned int m_vertexSize;
    unsigned int m_stride;
    unsigned int m_vao;
    unsigned int m_vbo;
};

} // end of namespace bot

#endif
