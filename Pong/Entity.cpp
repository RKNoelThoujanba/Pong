#include "include/Entity.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace Pong
{
    Entity::Entity(std::filesystem::path texPath, std::array<glm::vec4, 4> pos)
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

        void Entity::SetRotate(float angleRad, const glm::vec3& axis)
        {
            m_Rotation = glm::angleAxis(angleRad, axis);
        }

        void Entity::SetScale(const glm::vec3& scale)
        {
            m_Scale = scale;
        }

        void Entity::SetTranslate(const glm::vec3& translate)
        {
            m_Translate = translate;
        }

        void Entity::AddRotate(float angleRad, const glm::vec3& axis)
        {
            glm::quat additionalRotation = glm::angleAxis(angleRad, glm::normalize(axis));
            m_Rotation *= additionalRotation;
        }

        void Entity::AddScale(const glm::vec3& scale)
        {
            m_Scale *= scale;
        }

        void Entity::AddTranslate(const glm::vec3 translate)
        {
            m_Translate += translate;
        }

        const glm::mat4& Entity::GetModel(void) const
        {
            return m_Model;
        }

        const HGL::VertexArray& Entity::GetVertexArray(void) const
        {
            return m_VAO;
        }
        const HGL::VertexBuffer& Entity::GetVertexBuffer(void) const
        {
            return m_VBO;
        }
        const HGL::IndexBuffer& Entity::GetIndexBuffer(void) const
        {
            return m_IBO;
        }

        const HGL::Texture2D& Entity::GetTexture(void) const
        {
            return m_Texture;
        }

        const std::array<glm::vec2, 4>& Entity::GetWorldPosition(void) const
        {
            return m_WorldPos;
        }

        void Entity::UpdateModel()
        {
            m_Model = glm::translate(m_InitialModel, m_Translate);
            m_Model *= glm::mat4_cast(m_Rotation);
            m_Model = glm::scale(m_Model, m_Scale);

            size_t i = 0;

            for(auto& vertex : m_Pos)
            {
                glm::vec4 localPos = glm::vec4(vertex.x, vertex.y, 0.0f, 1.0f);
                glm::vec4 worldPos = m_Model * localPos;
                m_WorldPos[i++] = glm::vec2(worldPos.x, worldPos.y);
            }
        }
}