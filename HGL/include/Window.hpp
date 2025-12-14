#pragma once

#include "Core.hpp"
#include <string_view>

#include <exception>
#include <string>

namespace HGL
{
    enum class WindowError : uint8_t
    {
        NONE = 0,

        GLFW_INIT_FAILED,
        GLAD_LOAD_GL_FAILED,
        WINDOW_CREATION_FAILED
    };

    class GLDebugException : public std::exception
    {
    public:
        GLDebugException(std::string_view what)
            :m_what(what)
        {}

        const char* what(void) const noexcept override
        {
            return m_what.c_str();
        }
    private:
        std::string m_what;
    };

    //TODO: add move ctor?
    struct WindowProperties
    {
        int Width = 1280, Height = 720;
        std::string_view Title = "No Name";
        bool Resizeable = false;
        bool VSync = true;
        bool Fullscreen = false;
        bool DebugMode = true;
    };

    class Window
    {
    public:
        Window(const WindowProperties& properties, WindowError& err);
        Window(const Window& other) = delete;
        Window(Window&& other);
        ~Window();

        void SwapBuffers(void);
        void PollEvents(void);
        bool ShouldClose(void);

        int GetWidth(void) const;
        int GetHeight(void) const;
        Window& operator = (const Window&) = delete;
        Window& operator = (Window&& other)
        {
            this->m_window = other.m_window;
            this->m_windowProperties = std::move(other.m_windowProperties);

            other.m_window = nullptr;

            return *this;
        }
        
        
    private:
        GLFWwindow* m_window = nullptr;
        WindowProperties m_windowProperties;

        friend class EventHandler;
        
    };
}