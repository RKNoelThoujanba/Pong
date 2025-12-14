#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <exception>
#include <string>
#include <string_view>

namespace HGL
{
    class InvalidGLObjectBind : public std::exception
    {
    public:
        InvalidGLObjectBind(std::string_view what)
            :m_what(what)
        {}

        const char* what(void) const noexcept override
        {
            return m_what.c_str();
        }
    private:
        std::string m_what;
    };
}