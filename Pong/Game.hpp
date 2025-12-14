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

static std::unique_ptr<HGL::Window> s_Window = nullptr;
static std::unique_ptr<HGL::Shader> s_Shader = nullptr;
static std::unique_ptr<HGL::Texture2D> s_PaddleTexture, s_BallTexture, s_BackgroundTexture;

static std::vector<glm::vec4> s_GlobalPos = 
{
    glm::vec4(16.0f, 9.0f, 1.0f, 1.0f), // top right
    glm::vec4(16.0f, -9.0f, 1.0f, 0.0f), // bottom right
    glm::vec4(-16.0f, -9.0f, 0.0f, 0.0f), // bottom left
    glm::vec4(-16.0f, 9.0f, 0.0f, 1.0f) // top left
};

uint32_t s_GloablIndices[] = {0, 1, 3, 1, 2, 3};

namespace Pong
{
    void InitWindow(const std::vector<const char*>& args)
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

        windowProperties.Resizeable = true;

        //TODO: import vsync from cfg

        windowProperties.Title = "Pong";

        HGL::WindowError err;
        s_Window = std::make_unique<HGL::Window>(windowProperties, err);

        //TODO: err check
    }

    void InitGameResources(const std::vector<const char*>& args)
    {
        (void) args;

        s_Shader = std::make_unique<HGL::Shader>("./vertex.glsl", "./fragment.glsl");

        //TODO: set proper textures
        s_PaddleTexture = std::make_unique<HGL::Texture2D>("./brick.jpg");
        s_BallTexture = std::make_unique<HGL::Texture2D>("./brick.jpg");
        s_BackgroundTexture = std::make_unique<HGL::Texture2D>("./BG.jpg");
    }

    void RunGame(const std::vector<const char*>& args)
    {
        HGL::VertexArray VAO;
        HGL::VertexBufferLayout vbl;
        vbl.AddElements<float, 2>();
        vbl.AddElements<float, 2>();

        HGL::VertexBuffer vb(s_GlobalPos.data(), s_GlobalPos.size() * sizeof(float) * 4, HGL::VertexBuffer::Usage::DYNAMIC_DRAW);
        HGL::IndexBuffer ib(s_GloablIndices, 6, HGL::IndexBuffer::Usage::STATIC_DRAW);

        

        VAO.SetVertexBufferLayout(vbl);
        s_Shader->Bind();
        s_Shader->SetUniform<int>(std::string_view("u_TexSlot"), 0);
        s_BallTexture->Bind(0);

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f);

        s_Shader->SetUniformMat<float, 4>(std::string_view("u_Model"), glm::value_ptr(model[0]), 1, false);
        s_Shader->SetUniformMat<float, 4>(std::string_view("u_View"), glm::value_ptr(view[0]), 1, false);
        s_Shader->SetUniformMat<float, 4>(std::string_view("u_Projection"), glm::value_ptr(projection[0]), 1, false);
        


        while(!s_Window->ShouldClose())
        {
            glClear(GL_COLOR_BUFFER_BIT);
            

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            s_Window->SwapBuffers();
            s_Window->PollEvents();
        }
    }
}