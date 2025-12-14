#pragma once

#include "Core.hpp"
#include <string>
#include <string_view>
#include <filesystem>
#include <map>

#include <exception>
#include <string>
#include <string_view>
#include <expected>

namespace HGL
{

    enum class ShaderError
    {
        ERROR_NONE,
        VERTEX_SHADER_NOT_FOUND,
        FRAMGMENT_SHADER_NOT_FOUND,
        
        VERTEX_SHADER_COMPILATION_FAILED,
        FRAGMENT_SHADER_COMPILATION_FAILED,

        SHADER_LINK_FAILED
    };
    struct ShaderErrorInfo
    {
        ShaderError ErrorCode = ShaderError::ERROR_NONE;
        std::string ErrorMessage;
    };

    class UniformNotFoundException : public std::exception
    {
    public:
        UniformNotFoundException(std::string_view what)
            :m_what(what)
        {}

        const char* what(void) const noexcept override
        {
            return m_what.c_str();
        }

    private:
        std::string m_what;
    };

    class Shader
    {
    public:
    
        Shader() = default;
        Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath);
        Shader(const Shader&) = delete;
        Shader(Shader&& other);
        ~Shader();

        const ShaderErrorInfo& GetStatus(void) const;
        void Bind(void) const;
        
        Shader& operator = (const Shader&) = delete;
        Shader& operator = (Shader&& other)
        {
            this->m_programID = other.m_programID;
            other.m_programID = 0;

            return *this;
        }

        template<typename T>
        void SetUniform(std::string_view uniformName, T v0);

        template<typename T>
        void SetUniform(std::string_view uniformName, T v0, T v1);

        template<typename T>
        void SetUniform(std::string_view uniformName, T v0, T v1, T v2);

        template<typename T>
        void SetUniform(std::string_view uniformName, T v0, T v1, T v2, T v3);

        template<typename T, GLsizei N>
        void SetUniformV(std::string_view uniformName, const T* value, GLsizei count);

        template<typename T, GLsizei N>
        void SetUniformMat(const std::string_view uniformName, const T* mat, GLsizei count, bool transpose);

        template<typename T, GLsizei M, GLsizei N>
        void SetUniformMat(const std::string_view uniformName, const T* mat, GLsizei count, bool transpose);
        
    private:
        GLuint m_programID = 0;
        ShaderErrorInfo m_status;

        std::map<std::string, GLint> m_uniformMap;

        std::expected<GLint, std::string> GetUniformLocation(std::string_view name);
    };
}