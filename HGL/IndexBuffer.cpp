#include "include/IndexBuffer.hpp"

namespace HGL
{
    IndexBuffer::IndexBuffer()
    {
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    }
    IndexBuffer::IndexBuffer(const uint32_t* data, size_t count, Usage usage)
    {
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, (GLuint) usage);
    }
    IndexBuffer::IndexBuffer(IndexBuffer&& other)
    {
        this->m_IBO = other.m_IBO;
        other.m_IBO = 0;
    }
    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_IBO);
        m_IBO = 0;
    }

    void IndexBuffer::Bind(void) const
    {
        if(m_IBO == 0)
        {
            throw InvalidGLObjectBind("Tried to bind invalid IB");
            return;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    }

    void IndexBuffer::SetData(const uint32_t* data, size_t count, Usage usage)
    {
        this->Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, (GLuint) usage);
    }
}