#include "include/EventHandler.hpp"

namespace HGL
{
    EventHandler::EventHandler(Window& window)
        :m_Window(window)
    { }

    bool EventHandler::IsKeyPressed(Key key)
    {
        return glfwGetKey(m_Window.m_window, (int)key) == GLFW_PRESS;
    }

    bool EventHandler::IsKeyReleased(Key key)
    {
        return glfwGetKey(m_Window.m_window, (int)key) == GLFW_RELEASE;
    }

    bool EventHandler::IsKeyRepeated(Key key)
    {
        return glfwGetKey(m_Window.m_window, (int)key) == GLFW_REPEAT;
    }
}