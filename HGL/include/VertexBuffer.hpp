#pragma once

#include "Core.hpp"

namespace HGL
{

    class VertexBuffer
    {
        public:
        enum class Usage : GLenum
        {
            STATIC_DRAW = GL_STATIC_DRAW,
            DYNAMIC_DRAW = GL_DYNAMIC_DRAW
        };

        VertexBuffer();
        VertexBuffer(const void* data, size_t size, Usage usage);
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&& other);
        ~VertexBuffer();

        void Bind() const;
        void SetData(const void* data, size_t size, Usage usage);

        VertexBuffer& operator = (const VertexBuffer&) = delete;
        VertexBuffer& operator = (VertexBuffer&& other)
        {
            this->m_VBO = other.m_VBO;
            other.m_VBO = 0;

            return *this;
        }

    private:
        uint32_t m_VBO = 0;
    };
}