#pragma once

#include <iostream>

#include <memory>
#include <print>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <VertexArray.hpp>
#include <IndexBuffer.hpp>
#include "Entity.hpp"
#include "PhysicsObject.hpp"
#include <EventHandler.hpp>
#include "Random.hpp"



namespace Pong
{
    static constexpr glm::vec2 PADDLE_VELOCITY(0.0f, 10.0f);
    static constexpr glm::vec2 BALL_INITIAL_VELOCITY(5.0f, 5.0f);
    static constexpr glm::vec2 BALL_VELOCITY_INCREASE(0.2f, 0.2f);
    static constexpr float BALL_VELOCITY_COMPONENT_RANDOMNESS = 1.0f;
    class Game
    {
    public:
        Game(const std::vector<const char*>& args);

        virtual ~Game() = default;

        virtual void OnInit(void);

        virtual void Update(float dt);

        //translate->rotate->scale
        void RunGame();
    private:
        std::unique_ptr<PhysicsObject> m_Ball, m_Paddle1, m_Paddle2;
        std::unique_ptr<HGL::Window> m_Window = nullptr;
        std::unique_ptr<HGL::Shader> m_Shader = nullptr;
        std::unique_ptr<HGL::EventHandler> m_EventHandler = nullptr;

        float m_TimeEllapsed = 0.0f;

        uint16_t m_Player1Score = 0, m_Player2Score = 0;

        RNG m_RNG;

        bool m_IsPaused = true;
    };  
}