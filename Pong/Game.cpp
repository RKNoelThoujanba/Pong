#include "include/Game.hpp"
#include <chrono>
#include "include/Resolutions.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pong
{
    Game::Game(const std::vector<const char*>& args)
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
             //720p

            if(args.size() != 3)
            {
                throw std::runtime_error("Invalid args. Provide Resolution");
            }

            int width = std::stoi(args[1]);
            int height = std::stoi(args[2]);

            bool resFound = false;
            for(const auto& resolution : ValidResolutions)
            {
                if(width == resolution.Width && height == resolution.Height)
                {
                    resFound = true;
                    break;
                }
            }

            if(!resFound)
            {
                throw std::runtime_error("Invalid Resolution\n");
            }

            Resolution res = {width, height};

            windowProperties.Width = res.Width;
            windowProperties.Height = res.Height;

            windowProperties.Resizeable = false;

            //TODO: import vsync from cfg

            windowProperties.Title = "Pong";

            HGL::WindowError err;
            m_Window = std::make_unique<HGL::Window>(windowProperties, err);
            m_EventHandler = std::make_unique<HGL::EventHandler>(*m_Window);

            //TODO: err check

            m_Shader = std::make_unique<HGL::Shader>("./Resources/Shaders/vertex.glsl", "./Resources/Shaders/fragment.glsl");
        }

        void Game::OnInit(void) 
        {
            m_IsPaused = true;
            m_Paddle1 = std::make_unique<PhysicsObject>("./Resources/Textures/paddle.jpg");
            m_Paddle2 = std::make_unique<PhysicsObject>("./Resources/Textures/paddle.jpg");
            std::array<glm::vec4, 4> square = 
            {{
                glm::vec4(9.0f, 9.0f, 1.0f, 1.0f), // top right 0
                glm::vec4(9.0f, -9.0f, 1.0f, 0.0f), // bottom right 1
                glm::vec4(-9.0f, -9.0f, 0.0f, 0.0f), // bottom left 2 
                glm::vec4(-9.0f, 9.0f, 0.0f, 1.0f) // top left 3
            }};
            m_Ball = std::make_unique<PhysicsObject>("./Resources/Textures/ball.png", square);

            m_Paddle1->SetTranslate(glm::vec3(-12.0f, 0.0f, 0.0f));
            m_Paddle2->SetTranslate(glm::vec3(+12.0f, 0.0f, 0.0f));

            m_Paddle1->SetScale(glm::vec3(0.01f, 0.3f, 0.1f));
            m_Paddle2->SetScale(glm::vec3(0.01f, 0.3f, 0.1f));

            m_Ball->SetScale(glm::vec3(0.02f, 0.02f, 1.0f));
            m_Ball->SetVelocity(BALL_INITIAL_VELOCITY);
        }


        void Game::Update(float dt)
        {
            if(m_EventHandler->IsKeyPressed(HGL::Key::Escape))
                m_IsPaused = true;
            if(m_EventHandler->IsKeyPressed(HGL::Key::Space))
                m_IsPaused = false;
            if(m_IsPaused)
                return;

            if(dt > 1.0f/30.0f)
                dt = 1.0f/30.0f;
            m_Ball->Update(dt);
            

            m_Paddle1->UpdateModel();
            m_Paddle2->UpdateModel();
            m_Ball->UpdateModel();

            if(m_Ball->GetWorldPosition()[0].x >= 17.0f)
            {
                ++m_Player1Score;
                OnInit();
                return;
            }
            if(m_Ball->GetWorldPosition()[0].x <= -17.0f)
            {
                ++m_Player2Score;
                OnInit();
                return;
            }

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
                float p1x = paddle1Position[3].x;
                float p1y = paddle1Position[3].y;

                float p2x = paddle2Position[3].x;
                float p2y = paddle2Position[3].y;

                float bx = ballWorldCords[3].x;
                float by = ballWorldCords[3].y;

                glm::vec2 velocityMultipler(1.0f);
                if(m_Ball->GetVelocity().x == 0.0f)
                {
                    m_Ball->SetVelocity(glm::vec2(m_Ball->GetVelocity().x, m_Ball->GetVelocity().x));
                }
                
                
                float bm = m_Ball->GetVelocity().y / m_Ball->GetVelocity().x;
                float sol1x = p1x;
                float sol1y = bm * (p1x - bx) + by;

                float sol2x = p2x;
                float sol2y = bm * (p2x - bx) + by;

                bool isSol1Ceil, isSol2Ceil;

                isSol1Ceil = sol1y == m_Paddle1->GetWorldPosition()[0].y || sol1y == m_Paddle1->GetWorldPosition()[1].y;
                isSol2Ceil = sol2y == m_Paddle2->GetWorldPosition()[0].y || sol2y == m_Paddle2->GetWorldPosition()[1].y;

                auto getDistanceSquare = [](float x1, float y1, float x2, float y2) -> float
                {
                    return (x2 - x1) *  (x2 - x1) + (y2 - y1) * (y2 - y1);
                };

                if(getDistanceSquare(sol1x, sol1y, bx, by) < getDistanceSquare(sol2x, sol2y, bx, by))
                {
                    //sol1 is the nearest point
                    if(isSol1Ceil) velocityMultipler = glm::vec2(+1.0f, -1.0f);
                    else velocityMultipler = glm::vec2(-1.0f, +1.0f);
                }
                else
                {
                    if(isSol2Ceil) velocityMultipler = glm::vec2(+1.0f, -1.0f);
                    else velocityMultipler = glm::vec2(-1.0f, +1.0f);
                }
                    
                
                
                m_Ball->SetVelocity(m_Ball->GetVelocity() * velocityMultipler);
                
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
        void Game::RunGame()
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
}