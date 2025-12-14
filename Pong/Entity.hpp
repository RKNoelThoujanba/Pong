#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <VertexArray.hpp>
#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>
#include <Texture.hpp>
#include <string_view>

namespace Pong
{
    class Entity
    {
    public:
        Entity(std::string_view texPath)
            :m_Model(1.0f), 
            m_Pos
            (
                {
                    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
                    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
                    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
                    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
                }
            ),
            m_Texture(texPath.data())
        {
            uint32_t indices[] = {0, 1, 3, 1, 2, 3};

            m_VAO.Bind();
            HGL::VertexBufferLayout vbl;
            vbl.AddElements<float, 2>(); //x,y cords
            vbl.AddElements<float, 2>(); // tex cords

            m_VBO.SetData(m_Pos.data(), m_Pos.size() * sizeof(float) * 4, HGL::VertexBuffer::Usage::STATIC_DRAW);
            m_IBO.SetData(indices, 6, HGL::IndexBuffer::Usage::STATIC_DRAW);

            m_VAO.SetVertexBufferLayout(vbl);
        }

        void Rotate(float angleRad, const glm::vec3& axis)
        {
            m_Model = glm::rotate(m_Model, angleRad, axis);
        }

        void Scale(const glm::vec3& scale)
        {
            m_Model = glm::scale(m_Model, scale);
        }

        void Translate(const glm::vec3& translate)
        {
            m_Model = glm::translate(m_Model, translate);
        }

        const glm::mat4& GetModel(void) const
        {
            return m_Model;
        }

        const HGL::VertexArray& GetVertexArray(void) const
        {
            return m_VAO;
        }
        const HGL::VertexBuffer& GetVertexBuffer(void) const
        {
            return m_VBO;
        }
        const HGL::IndexBuffer& GetIndexBuffer(void) const
        {
            return m_IBO;
        }

        const HGL::Texture2D& GetTexture(void) const
        {
            return m_Texture;
        }
        

    private:
        glm::mat4 m_Model;
        std::vector<glm::vec4> m_Pos;
        
        HGL::VertexArray m_VAO;
        HGL::VertexBuffer m_VBO;
        HGL::IndexBuffer m_IBO;
        HGL::Texture2D m_Texture;
    };
}