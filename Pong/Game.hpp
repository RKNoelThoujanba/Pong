#pragma once

#include <iostream>
#include "Resolutions.hpp"
#include <memory>
#include <print>
#include <vector>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>
#include <VertexArray.hpp>
#include <IndexBuffer.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.hpp"
#include <chrono>
#include "PhysicsObject.hpp"
#include <EventHandler.hpp>
#include "Random.hpp"



namespace Pong
{
    static constexpr glm::vec2 PADDLE_VELOCITY(0.0f, 10.0f);
    static constexpr glm::vec2 BALL_INITIAL_VELOCITY(5.0f, 5.0f);
    static constexpr glm::vec2 BALL_VELOCITY_INCREASE(0.2f, 0.2f);
    static constexpr float BALL_VELOCITY_COMPONENT_RANDOMNESS = 2.0f;
    class Game
    {
    public:
        Game(const std::vector<const char*>& args)
        {
            (void) args;

            HGL::WindowProperties windowProperties;
            #ifdef NDEBUG
            windowProperties.DebugMode = false;
            std::print("NDEBUG");
            #else
            windowProperties.DebugMode = true;
            std::print("DEBUG");
            #endif

            //TODO: import resolutions from cfg file
            Resolution res = ValidResolutions[0]; //720p
            windowProperties.Width = res.Width;
            windowProperties.Height = res.Height;

            windowProperties.Resizeable = false;

            //TODO: import vsync from cfg

            windowProperties.Title = "Pong";

            HGL::WindowError err;
            m_Window = std::make_unique<HGL::Window>(windowProperties, err);
            m_EventHandler = std::make_unique<HGL::EventHandler>(*m_Window);

            //TODO: err check

            m_Shader = std::make_unique<HGL::Shader>("./vertex.glsl", "./fragment.glsl");
        }

        virtual ~Game() = default;

        virtual void OnInit(void) 
        {
            m_Paddle1 = std::make_unique<PhysicsObject>("./brick.jpg");
            m_Paddle2 = std::make_unique<PhysicsObject>("./brick.jpg");
            std::array<glm::vec4, 4> square = 
            {{
                glm::vec4(9.0f, 9.0f, 1.0f, 1.0f), // top right 0
                glm::vec4(9.0f, -9.0f, 1.0f, 0.0f), // bottom right 1
                glm::vec4(-9.0f, -9.0f, 0.0f, 0.0f), // bottom left 2 
                glm::vec4(-9.0f, 9.0f, 0.0f, 1.0f) // top left 3
            }};
            m_Ball = std::make_unique<PhysicsObject>("./ball.jpg", square);

            m_Paddle1->SetTranslate(glm::vec3(-12.0f, 0.0f, 0.0f));
            m_Paddle2->SetTranslate(glm::vec3(+12.0f, 0.0f, 0.0f));

            m_Paddle1->SetScale(glm::vec3(0.01f, 0.3f, 0.1f));
            m_Paddle2->SetScale(glm::vec3(0.01f, 0.3f, 0.1f));

            m_Ball->SetScale(glm::vec3(0.02f, 0.02f, 1.0f));
            m_Ball->SetVelocity(BALL_INITIAL_VELOCITY);
        }


        virtual void Update(float dt)
        {
            m_Ball->Update(dt);
            

            m_Paddle1->UpdateModel();
            m_Paddle2->UpdateModel();
            m_Ball->UpdateModel();

            const auto& paddle1Position = m_Paddle1->GetWorldPosition();
            const auto& paddle2Position = m_Paddle2->GetWorldPosition();

            if(m_EventHandler->IsKeyPressed(HGL::Key::W) && paddle1Position[0].y <= 9.0f)
            {
                m_Paddle1->AddTranslate(glm::vec3(+PADDLE_VELOCITY * dt, 0.0f));
            }
            if(m_EventHandler->IsKeyPressed(HGL::Key::S) && paddle1Position[1].y >= -9.0f)
            {
                m_Paddle1->AddTranslate(glm::vec3(glm::vec3(-PADDLE_VELOCITY * dt, 0.0f)));
            }

            if(m_EventHandler->IsKeyPressed(HGL::Key::Up) && paddle2Position[0].y <= 9.0f)
            {
                m_Paddle2->AddTranslate(glm::vec3(+PADDLE_VELOCITY * dt, 0.0f));
            }
            if(m_EventHandler->IsKeyPressed(HGL::Key::Down) && paddle2Position[1].y >= -9.0f)
            {
                m_Paddle2->AddTranslate(glm::vec3(-PADDLE_VELOCITY * dt, 0.0f));
            }
            
            const std::array<glm::vec2, 4>& ballWorldCords = m_Ball->GetWorldPosition();
            
            
            if(ballWorldCords[0].y >= +9.0f || ballWorldCords[1].y <= -9.0f)
            {
                m_Ball->SetVelocity(m_Ball->GetVelocity() * glm::vec2(1.0f, -1.0f));
            }

            if(m_Ball->CollidesWith(*m_Paddle1) || m_Ball->CollidesWith(*m_Paddle2))
            {
                m_Ball->SetVelocity(m_Ball->GetVelocity() * glm::vec2(-1.0f, 1.0f));
                m_Ball->SetVelocity(
                    m_Ball->GetVelocity() + m_Ball->GetVelocity() * BALL_VELOCITY_INCREASE + 
                    glm::vec2
                    (
                        m_RNG.UniformDistribution<float>(-BALL_VELOCITY_COMPONENT_RANDOMNESS, +BALL_VELOCITY_COMPONENT_RANDOMNESS),
                        m_RNG.UniformDistribution<float>(-BALL_VELOCITY_COMPONENT_RANDOMNESS, +BALL_VELOCITY_COMPONENT_RANDOMNESS)
                    )
                );
            }
        }

        //translate->rotate->scale
        void RunGame()
        {

            OnInit();

            m_Shader->Bind();
            m_Shader->SetUniform<int>(std::string_view("u_TexSlot"), 0);

            
            glm::mat4 view(1.0f);
            glm::mat4 projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f);

            m_Shader->SetUniformMat<float, 4>(std::string_view("u_View"), glm::value_ptr(view[0]), 1, false);
            m_Shader->SetUniformMat<float, 4>(std::string_view("u_Projection"), glm::value_ptr(projection[0]), 1, false);

            std::chrono::high_resolution_clock::time_point prev = std::chrono::high_resolution_clock::now() ,current = std::chrono::high_resolution_clock::now();
            while(!m_Window->ShouldClose())
            {
                
                glClear(GL_COLOR_BUFFER_BIT);

                current = std::chrono::high_resolution_clock::now();
                float dt = std::chrono::duration<float>(current - prev).count();

                auto renderEntity = [&, this] (Entity& e)
                {
                    m_Shader->SetUniformMat<float, 4>(std::string_view("u_Model"), glm::value_ptr(e.GetModel()[0]), 1, false);
                    e.UpdateModel();
                    e.GetVertexArray().Bind();
                    e.GetIndexBuffer().Bind();
                    e.GetTexture().Bind(0);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                };

                Update(dt);

                renderEntity(*m_Paddle1);
                renderEntity(*m_Paddle2);
                renderEntity(*m_Ball);
                
                
                m_Window->SwapBuffers();
                m_Window->PollEvents();

                
                prev = current;

                m_TimeEllapsed += dt;
                // /std::println("FPS: {}", 1.0f/dt);
            }
        }
    private:
        std::unique_ptr<PhysicsObject> m_Ball, m_Paddle1, m_Paddle2;
        std::unique_ptr<HGL::Window> m_Window = nullptr;
        std::unique_ptr<HGL::Shader> m_Shader = nullptr;
        std::unique_ptr<HGL::EventHandler> m_EventHandler = nullptr;

        float m_TimeEllapsed = 0.0f;

        RNG m_RNG;
    };  
}