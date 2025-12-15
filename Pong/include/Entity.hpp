#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/matrix.hpp>
#include <filesystem>
#include <VertexArray.hpp>
#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>
#include <Texture.hpp>

namespace Pong
{
    class Entity
    {
    public:
        Entity
        (
            std::filesystem::path texPath, 
            std::array<glm::vec4, 4> pos = {{
                    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
                    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
                    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
                    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
                }}
        );

        virtual ~Entity() = default;

        void SetRotate(float angleRad, const glm::vec3& axis);

        void SetScale(const glm::vec3& scale);

        void SetTranslate(const glm::vec3& translate);

        void AddRotate(float angleRad, const glm::vec3& axis);

        void AddScale(const glm::vec3& scale);

        void AddTranslate(const glm::vec3 translate);

        const glm::mat4& GetModel(void) const;

        const HGL::VertexArray& GetVertexArray(void) const;
        const HGL::VertexBuffer& GetVertexBuffer(void) const;
        const HGL::IndexBuffer& GetIndexBuffer(void) const;
        const HGL::Texture2D& GetTexture(void) const;
        const std::array<glm::vec2, 4>& GetWorldPosition(void) const;

        void UpdateModel();
        
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