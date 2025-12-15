#pragma once

#include "Entity.hpp"
#include <array>

namespace Pong
{
    class PhysicsObject : public Entity
    {
    public:
        PhysicsObject
        (
            std::filesystem::path texPath, 
            std::array<glm::vec4, 4> pos = {{
                    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
                    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
                    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
                    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
                }}
        );
        const glm::vec2& GetVelocity(void) const;
        void SetVelocity(const glm::vec2 velocity);
        bool CollidesWith(const PhysicsObject& other) ;
        void Update(float dt);
    private:
        glm::vec2 m_Velocity;

    };
}