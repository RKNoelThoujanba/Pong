#include "include/VertexArray.hpp"

namespace HGL
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

    VertexArray::VertexArray(VertexArray &&other)
    {
        this->m_VAO = other.m_VAO;
        this->m_enbaleIndicesCount = other.m_enbaleIndicesCount;

        other.m_VAO = 0;
        other.m_enbaleIndicesCount = 0;
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_VAO);
    }

    void VertexArray::Bind(void) const
    {
        if(m_VAO == 0)
        {
            throw InvalidGLObjectBind("Tried to bind invalid VA");
            return;
        }
        glBindVertexArray(m_VAO);
    }

    void VertexArray::SetVertexBufferLayout(const VertexBufferLayout& vbl)
    {
        this->Bind();
        for(GLsizei i = 0; i < m_enbaleIndicesCount; ++i)
        {
            glDisableVertexAttribArray(i);
        }
        m_enbaleIndicesCount = 0;

        const void* offset = 0;
        for(const auto& vbe : vbl.GetVertexLayout())
        {
            GLsizei stride = vbl.GetStride();
            GLsizei offsetInc = vbe.Count * GetVBLTypeSize(vbe.Type);
            glEnableVertexAttribArray(m_enbaleIndicesCount);
            glVertexAttribPointer(m_enbaleIndicesCount, vbe.Count, (GLenum) vbe.Type, vbe.Type == VBLDataTypes::F32 || vbe.Type == VBLDataTypes::F64 ? GL_FALSE : GL_TRUE, stride, offset);
            
            offset = reinterpret_cast<const void*>(reinterpret_cast<size_t>(offset) + offsetInc);

            ++m_enbaleIndicesCount;
        }
    }
}