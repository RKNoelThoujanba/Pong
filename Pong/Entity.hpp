#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <VertexArray.hpp>
#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>
#include <Texture.hpp>
#include <string_view>
#include <glm/gtc/quaternion.hpp>

namespace Pong
{
    class Entity
    {
    public:
        Entity(
            std::filesystem::path texPath, 
            std::array<glm::vec4, 4> pos = {{
                    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
                    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
                    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
                    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
                }}
            )
            :m_InitialModel(1.0f), m_Texture(texPath), m_Pos(pos), m_Translate(0.0f), m_Rotation(0.0f, glm::vec3(0.0f)), m_Scale(1.0f)
        {
            uint32_t indices[] = {0, 1, 3, 1, 2, 3};

            m_VAO.Bind();
            HGL::VertexBufferLayout vbl;
            vbl.AddElements<float, 2>(); //x,y cords
            vbl.AddElements<float, 2>(); // tex cords
            m_VBO.Bind();
            m_VAO.SetVertexBufferLayout(vbl);

            m_VBO.SetData(m_Pos.data(), m_Pos.size() * sizeof(float) * 4, HGL::VertexBuffer::Usage::STATIC_DRAW);
            m_IBO.SetData(indices, 6, HGL::IndexBuffer::Usage::STATIC_DRAW);

            for(size_t i = 0; i < 4; ++i)
            {
                m_WorldPos[i] = glm::vec2(pos[i].x, pos[i].y);
            }
        }

        virtual ~Entity() = default;

        void SetRotate(float angleRad, const glm::vec3& axis)
        {
            m_Rotation = glm::angleAxis(angleRad, axis);
        }

        void SetScale(const glm::vec3& scale)
        {
            m_Scale = scale;
        }

        void SetTranslate(const glm::vec3& translate)
        {
            m_Translate = translate;
        }

        void AddRotate(float angleRad, const glm::vec3& axis)
        {
            glm::quat additionalRotation = glm::angleAxis(angleRad, glm::normalize(axis));
            m_Rotation *= additionalRotation;
        }

        void AddScale(const glm::vec3& scale)
        {
            m_Scale *= scale;
        }

        void AddTranslate(const glm::vec3 translate)
        {
            m_Translate += translate;
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

        const std::array<glm::vec2, 4>& GetWorldPosition(void) const
        {
            return m_WorldPos;
        }

        void UpdateModel()
        {
            m_Model = glm::translate(m_InitialModel, m_Translate);
            m_Model *= glm::mat4_cast(m_Rotation);
            m_Model = glm::scale(m_Model, m_Scale);

            size_t i = 0;

            for(auto& vertex : m_Pos)
            {
                glm::vec4 localPos = glm::vec4(vertex.x, vertex.y, 0.0f, 1.0f);
                glm::vec4 worldPos = m_Model * localPos;

                std::println("({0}, {1}, {2}, {3})",worldPos.x, worldPos.y, worldPos.z, worldPos.w);

                m_WorldPos[i++] = glm::vec2(worldPos.x, worldPos.y);
            }
        }
        
    private:
        glm::mat4 m_InitialModel, m_Model;
        
        HGL::VertexArray m_VAO;
        HGL::VertexBuffer m_VBO;
        HGL::IndexBuffer m_IBO;
        HGL::Texture2D m_Texture;
        std::array<glm::vec4, 4> m_Pos;
        glm::vec3 m_Translate;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;

        std::array<glm::vec2, 4> m_WorldPos;
    };
}