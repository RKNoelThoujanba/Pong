#pragma once

#include "Core.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace HGL
{
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(const VertexArray&) = delete;
        VertexArray(VertexArray&& other);
        ~VertexArray();

        void Bind(void) const;

        void SetVertexBufferLayout(const VertexBufferLayout& vbl);

        VertexArray& operator = (const VertexArray&) = delete;
        VertexArray& operator = (VertexArray&& other)
        {
            this->m_VAO = other.m_VAO;
            this->m_enbaleIndicesCount = other.m_enbaleIndicesCount;

            other.m_VAO = 0;
            other.m_enbaleIndicesCount = 0;

            return *this;
        }

    private:
        GLuint m_VAO;
        GLsizei m_enbaleIndicesCount = 0;
    };
}
