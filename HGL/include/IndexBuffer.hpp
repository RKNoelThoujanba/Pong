#pragma once

#include "Core.hpp"

namespace HGL
{
    class IndexBuffer
    {
        public:
        enum class Usage : GLenum
        {
            STATIC_DRAW = GL_STATIC_DRAW,
            DYNAMIC_DRAW = GL_DYNAMIC_DRAW
        };

        IndexBuffer();
        IndexBuffer(const uint32_t* data, size_t count, Usage usage);
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&& other);
        ~IndexBuffer();

        void Bind() const;
        void SetData(const uint32_t* data, size_t count, Usage usage);

        IndexBuffer& operator = (const IndexBuffer&) = delete;
        IndexBuffer& operator = (IndexBuffer&& other)
        {
            this->m_IBO = other.m_IBO;
            other.m_IBO = 0;

            return *this;
        }

    private:
        uint32_t m_IBO = 0;
    };
}