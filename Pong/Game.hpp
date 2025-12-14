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

static std::unique_ptr<HGL::Window> s_Window = nullptr;
static std::unique_ptr<HGL::Shader> s_Shader = nullptr;


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

        windowProperties.Resizeable = false;

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
    }

    //translate->rotate->scale
    void RunGame(const std::vector<const char*>& args)
    {
        Entity e("./brick.jpg");
        e.Translate(glm::vec3(5.0f, 2.0f, 0.0f));
        e.Rotate(3.1415, glm::vec3(0.0f, 1.0f, 0.0f));
        e.Scale(glm::vec3(0.5f, 0.5f, 1.0f));


        s_Shader->Bind();
        s_Shader->SetUniform<int>(std::string_view("u_TexSlot"), 0);

        
        glm::mat4 view(1.0f);
        glm::mat4 projection = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f);

        s_Shader->SetUniformMat<float, 4>(std::string_view("u_View"), glm::value_ptr(view[0]), 1, false);
        s_Shader->SetUniformMat<float, 4>(std::string_view("u_Projection"), glm::value_ptr(projection[0]), 1, false);
        
        s_Shader->SetUniformMat<float, 4>(std::string_view("u_Model"), glm::value_ptr(e.GetModel()[0]), 1, false);

        e.GetVertexArray().Bind();
        e.GetIndexBuffer().Bind();
        e.GetTexture().Bind(0);
        

        while(!s_Window->ShouldClose())
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            s_Window->SwapBuffers();
            s_Window->PollEvents();
        }
    }
}