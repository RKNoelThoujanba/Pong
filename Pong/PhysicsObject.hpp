#pragma once

#include "Entity.hpp"
#include <array>

namespace Pong
{
    class PhysicsObject : public Entity
    {
    public:
        PhysicsObject(
            std::filesystem::path texPath, 
            std::array<glm::vec4, 4> pos = {{
                    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
                    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
                    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
                    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
                }}
            )
            :Entity(texPath, pos), m_Velocity(glm::vec2(0.0f, 0.0f))
        {}

        const glm::vec2& GetVelocity(void) const
        {
            return m_Velocity;
        }
        void SetVelocity(const glm::vec2 velocity)
        {
            m_Velocity = velocity;
        }

        bool CollidesWith(const PhysicsObject& other) 
        {
            // Get world positions (your actual function)
            const auto& pos1 = this->GetWorldPosition();
            const auto& pos2 = other.GetWorldPosition();
            
            // Find true AABB bounds from all 4 corners
            float r1_left = 1e9, r1_right = -1e9, r1_top = 1e9, r1_bottom = -1e9;
            float r2_left = 1e9, r2_right = -1e9, r2_top = 1e9, r2_bottom = -1e9;
            
            // Object 1 bounds
            for(const auto& p : pos1) {
                r1_left   = std::min(r1_left,   p.x);
                r1_right  = std::max(r1_right,  p.x);
                r1_top    = std::min(r1_top,    p.y);
                r1_bottom = std::max(r1_bottom, p.y);
            }
            
            // Object 2 bounds
            for(const auto& p : pos2) {
                r2_left   = std::min(r2_left,   p.x);
                r2_right  = std::max(r2_right,  p.x);
                r2_top    = std::min(r2_top,    p.y);
                r2_bottom = std::max(r2_bottom, p.y);
            }
            
            // Standard AABB collision test
            return (r1_right  >= r2_left  && r1_left   <= r2_right  &&
                    r1_bottom >= r2_top    && r1_top    <= r2_bottom);
        }

        void Update(float dt)
        {
            this->AddTranslate(glm::vec3(m_Velocity.x * dt, m_Velocity.y * dt, 0.0f));
        }
    private:
        glm::vec2 m_Velocity;

    };
}