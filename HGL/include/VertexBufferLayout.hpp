#pragma once

#include "Core.hpp"
#include <stdexcept>
#include <vector>

namespace HGL
{

    enum class VBLDataTypes : GLenum
    {
        UNSUPPORTED, U8 = GL_UNSIGNED_BYTE, U16 = GL_UNSIGNED_SHORT, U32 =  GL_UNSIGNED_INT, I8 = GL_BYTE, I16 = GL_SHORT, I32 = GL_INT, F32 = GL_FLOAT, F64 = GL_DOUBLE
    };

    inline GLsizei GetVBLTypeSize(VBLDataTypes type) // in bytes
    {
        switch(type)
        {
            case VBLDataTypes::UNSUPPORTED: return 0;

            case VBLDataTypes::U8:
            case VBLDataTypes::I8: return 1;

            case VBLDataTypes::U16:
            case VBLDataTypes::I16: return 2;

            case VBLDataTypes::F32:
            case VBLDataTypes::U32:
            case VBLDataTypes::I32: return 4; 

            case VBLDataTypes::F64: return 8;

            default:
                throw std::runtime_error("Invalid VBLDataType. Now is a very good time to panic");
                return 0;
        }
    }

    class VertexBufferLayout
    {
    public:
        struct VertexLayout
        {
            VertexLayout(VBLDataTypes type, GLsizei n)
                :Type(type), Count(n)
            { }
            VBLDataTypes Type;
            GLsizei Count;
        };
        VertexBufferLayout() = default;

        template<typename T, GLsizei N>
        void AddElements(void)
        {
            if constexpr(std::is_same<T, float>())
            {
                m_Layout.emplace_back(VBLDataTypes::F32, N);
            }
            else if constexpr(std::is_same<T, double>())
            {
                m_Layout.emplace_back(VBLDataTypes::F64, N);
            }
            else if constexpr(std::is_same<T, uint8_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::U8, N);
            }
            else if constexpr(std::is_same<T, int8_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::I8, N);
            }
            else if constexpr(std::is_same<T, uint16_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::U16, N);
            }
            else if constexpr(std::is_same<T, int16_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::I16, N);
            }
            else if constexpr(std::is_same<T, uint32_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::U32, N);
            }
            else if constexpr(std::is_same<T, int32_t>())
            {
                m_Layout.emplace_back(VBLDataTypes::I32, N);
            }
            else
            {
                static_assert(true, "Unsupported type");
            }
        }
    
        GLsizei GetStride(void) const
        {
            GLsizei stride = 0;
            for(const auto& element : m_Layout)
            {
                stride += element.Count * GetVBLTypeSize(element.Type);
            }

            return stride;
        }

        const std::vector<VertexLayout>& GetVertexLayout(void) const
        {
            return m_Layout;
        }
        
    private:
        std::vector<VertexLayout> m_Layout;
    };
}