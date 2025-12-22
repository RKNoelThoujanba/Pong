#include "include/Window.hpp"
#include <print>

namespace HGL
{
    static void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *data)
    {
        (void) length;
        (void)data;
        const char* sourceStr;
        const char* typeStr;
        const char* severityStr;
        switch (source) {
            case GL_DEBUG_SOURCE_API:
                sourceStr = "API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                sourceStr = "WINDOW SYSTEM";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                sourceStr = "SHADER COMPILER";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                sourceStr = "THIRD PARTY";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                sourceStr = "APPLICATION";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                sourceStr = "UNKNOWN";
                break;
            default:
                sourceStr = "UNKNOWN";
        }
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                typeStr = "ERROR";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeStr = "DEPRECATED BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeStr = "UDEFINED BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                typeStr = "PORTABILITY";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                typeStr = "PERFORMANCE";
                break;
            case GL_DEBUG_TYPE_OTHER:
                typeStr = "OTHER";
                break;
            case GL_DEBUG_TYPE_MARKER:
                typeStr = "MARKER";
                break;
            default:
                typeStr = "UNKNOWN";
        }
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                severityStr = "HIGH";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                severityStr = "MEDIUM";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                severityStr = "LOW";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                severityStr = "NOTIFICATION";
                break;
            default:
                severityStr = "UNKNOWN";
        }

        std::println("{0}: {1} of {2} severity, raised from {3}: {4}", id, typeStr, severityStr, sourceStr, msg);
        if(severity != GL_DEBUG_SEVERITY_NOTIFICATION) throw GLDebugException("GL ERROR!");
        
    }

    Window::Window(const WindowProperties& properties , WindowError& err)
        :m_windowProperties(properties)
    {
        if(!glfwInit())
        {
            err = WindowError::GLFW_INIT_FAILED;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, properties.DebugMode ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, properties.Resizeable ? GLFW_TRUE : GLFW_FALSE);

        if(m_windowProperties.Width == 0 && m_windowProperties.Height == 0)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            m_windowProperties.Width = mode->width;
            m_windowProperties.Height = mode->height;
            m_windowProperties.Fullscreen = true;
        }

        m_window = glfwCreateWindow(
            m_windowProperties.Width, m_windowProperties.Height, 
            m_windowProperties.Title.data(), 
            m_windowProperties.Fullscreen ? glfwGetPrimaryMonitor() : static_cast<GLFWmonitor*>(nullptr), 
            nullptr
        );
        if(m_window == nullptr)
        {
            err = WindowError::WINDOW_CREATION_FAILED;
            return;
        }
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(static_cast<int>(m_windowProperties.VSync));

        if(!gladLoadGL(glfwGetProcAddress))
        {
            err = WindowError::GLAD_LOAD_GL_FAILED;
            return;
        }

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        m_windowProperties.Width = fbWidth;
        m_windowProperties.Height = fbHeight;
        glViewport(0, 0, fbWidth, fbHeight);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int width, int height)
        {
            glViewport(0, 0, width, height);
        });

        if(m_windowProperties.DebugMode)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugMessageCallback, nullptr);
        }
        err = WindowError::NONE;
    }

    Window::Window(Window &&other)
    {
        this->m_window = other.m_window;
        this->m_windowProperties = std::move(other.m_windowProperties);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::SwapBuffers(void)
    {
        glfwSwapBuffers(m_window);
    }

    void Window::PollEvents(void)
    {
        glfwPollEvents();
    }

    bool Window::ShouldClose(void)
    {
        return glfwWindowShouldClose(m_window);
    }

    int Window::GetWidth(void) const
    {
        return m_windowProperties.Width;
    }
    int Window::GetHeight(void) const
    {
        return m_windowProperties.Height;
    }
}

