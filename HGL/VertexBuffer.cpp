#include "include/VertexBuffer.hpp"

namespace HGL
{
    VertexBuffer::VertexBuffer()
    {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    VertexBuffer::VertexBuffer(const void* data, size_t size, Usage usage)
    {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum) usage);
    }    
    VertexBuffer::VertexBuffer(VertexBuffer&& other)
    {
        this->m_VBO = other.m_VBO;
        other.m_VBO = 0;
    }
    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }

    void VertexBuffer::Bind(void) const
    {
        if(m_VBO == 0)
        {
            throw InvalidGLObjectBind("Tried to bind invalid VB");
            return;
        }
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    void VertexBuffer::SetData(const void* data, size_t size, Usage usage)
    {
        this->Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)usage);
    }
}
