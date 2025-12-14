#include "include/Shader.hpp"

#include <fstream>
#include <format>

namespace HGL
{
    static std::expected<uint32_t, ShaderErrorInfo> s_CreateShader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
    {
        std::ifstream vertShaderFile(vertexShaderPath, std::ifstream::in);
        if(!vertShaderFile.is_open())
            return std::unexpected<ShaderErrorInfo>({ShaderError::VERTEX_SHADER_NOT_FOUND, "Failed to read vertex shader file"});

        std::string vertSrc(std::istreambuf_iterator<char>(vertShaderFile), {});

        std::ifstream fragShaderFile(fragmentShaderPath, std::ifstream::in);
        if(!fragShaderFile.is_open())
        {
           return std::unexpected<ShaderErrorInfo>({ShaderError::FRAMGMENT_SHADER_NOT_FOUND ,"Failed to read fragment shader file"});
        }
        std::string fragSrc(std::istreambuf_iterator<char>(fragShaderFile), {});

        const char* vertRawSrc = vertSrc.c_str();
        uint32_t vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertRawSrc, nullptr);
        glCompileShader(vertShader);


        constexpr GLsizei LOG_BUFFER_SZ = 512;
        int success;
        char infoLog[LOG_BUFFER_SZ];

        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertShader, LOG_BUFFER_SZ - 1, NULL, infoLog);
            return std::unexpected<ShaderErrorInfo>({ShaderError::VERTEX_SHADER_COMPILATION_FAILED, infoLog});
        }

        const char* fragRawSrc = fragSrc.c_str();
        uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragRawSrc, nullptr);
        glCompileShader(fragShader);

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            return std::unexpected<ShaderErrorInfo>({ShaderError::FRAGMENT_SHADER_COMPILATION_FAILED, infoLog});
        }

        uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) 
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            return std::unexpected<ShaderErrorInfo>({ShaderError::SHADER_LINK_FAILED, infoLog});
        }

        return shaderProgram;

    }
    Shader::Shader(const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
    {
        auto result = s_CreateShader(vertexShaderPath, fragmentShaderPath);
        if(result.has_value())
        {
            m_programID = result.value();
            glUseProgram(m_programID);
        }
        else
        {
            m_status = result.error();
        }
    }

    Shader::Shader(Shader&& other)
    {
        this->m_programID = other.m_programID;
        other.m_programID = 0;
    }
    Shader::~Shader()
    {
        glDeleteProgram(m_programID);
        m_programID = 0;
    }

    const ShaderErrorInfo& Shader::GetStatus(void) const
    {
        return m_status;
    }

    void Shader::Bind(void) const
    {
        if(this->GetStatus().ErrorCode != ShaderError::ERROR_NONE)
        {
            throw InvalidGLObjectBind("Tried to bind invalid shader");
            return;
        }
        glUseProgram(m_programID);
    }


    std::expected<GLint, std::string> Shader::GetUniformLocation(std::string_view name)
    {
        //std::map<std::string, T>::find(const std::string&) seems to not have an overload for std::string_view
        auto it = m_uniformMap.find(std::string(name));
        if(it == m_uniformMap.end())
        {
            GLint location = glGetUniformLocation(m_programID, &name[0]);
            if(location == -1)
                return std::unexpected<std::string>(std::format("No uniform {0} in shader (ID = {1})", name, m_programID));
            
            m_uniformMap.emplace(std::string(name), location);

            return location;
        }
        else
        {
            const auto& [uniformName, uniformLocation] = *it;
            (void) uniformName;

            return uniformLocation;
        }
    }


    //

    template<typename T>
    void Shader::SetUniform(std::string_view uniformName, T v0)
    {
        (void) uniformName;
        (void) v0;
        static_assert(true);
    }

    template<typename T>
    void Shader::SetUniform(std::string_view uniformName, T v0, T v1)
    {
        (void) uniformName;
        (void) v0;
        (void) v1;
        static_assert(true);
    }

    template<typename T>
    void Shader::SetUniform(std::string_view uniformName, T v0, T v1, T v2)
    {
        (void) uniformName;
        (void) v0;
        (void) v1;
        (void) v2;
        static_assert(true);
    }

    template<typename T>
    void Shader::SetUniform(std::string_view uniformName, T v0, T v1, T v2, T v3)
    {
        (void) uniformName;
        (void) v0;
        (void) v1;
        (void) v2;
        (void) v3;
        static_assert(true);
    }

    template<typename T, GLsizei N>
    void Shader::SetUniformV(std::string_view uniformName, const T* value, GLsizei count)
    {
        (void) uniformName;
        (void) value;
        (void) count;
        static_assert(true);
    }

    template<typename T, GLsizei N>
    void Shader::SetUniformMat(const std::string_view uniformName, const T* mat, GLsizei count, bool transpose)
    {
        (void) uniformName;
        (void) mat;
        (void) count;
        (void) transpose;
        static_assert(true);
    }

    template<typename T, GLsizei M, GLsizei N>
    void Shader::SetUniformMat(const std::string_view uniformName, const T* mat, GLsizei count, bool transpose)
    {
        (void) uniformName;
        (void) mat;
        (void) count;
        (void) transpose;
        static_assert(true);
    }

    //

    template<>
    void Shader::SetUniform(std::string_view uniformName, float v0)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1f(result.value(), v0);
    }


    template<>
    void Shader::SetUniform(std::string_view uniformName, int v0)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1i(result.value(), v0);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, unsigned int v0)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1ui(result.value(), v0);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, double v0)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1d(result.value(), v0);
    }


    //-------------------------------------------------------------

    template<>
    void Shader::SetUniform(std::string_view uniformName, float v0, float v1)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2f(result.value(), v0, v1);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, int v0, int v1)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2i(result.value(), v0, v1);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, unsigned int v0, unsigned int v1)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2ui(result.value(), v0, v1);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, double v0, double v1)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2d(result.value(), v0, v1);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniform(std::string_view uniformName, float v0, float v1, float v2)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3f(result.value(), v0, v1, v2);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, int v0, int v1, int v2)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3i(result.value(), v0, v1, v2);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, unsigned int v0, unsigned int v1, unsigned int v2)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3ui(result.value(), v0, v1, v2);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, double v0, double v1, double v2)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3d(result.value(), v0, v1, v2);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniform(std::string_view uniformName, float v0, float v1, float v2, float v3)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4f(result.value(), v0, v1, v2, v3);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, int v0, int v1, int v2, int v3)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4i(result.value(), v0, v1, v2, v3);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4ui(result.value(), v0, v1, v2, v3);
    }

    template<>
    void Shader::SetUniform(std::string_view uniformName, double v0, double v1, double v2, double v3)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4d(result.value(), v0, v1, v2, v3);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformV<float, 1>(std::string_view uniformName, const float* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1fv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<float, 2>(std::string_view uniformName, const float* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2fv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<float, 3>(std::string_view uniformName, const float* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3fv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<float, 4>(std::string_view uniformName, const float* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4fv(result.value(), count, value);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformV<int, 1>(std::string_view uniformName, const int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1iv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<int, 2>(std::string_view uniformName, const int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2iv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<int, 3>(std::string_view uniformName, const int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3iv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<int, 4>(std::string_view uniformName, const int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4iv(result.value(), count, value);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformV<unsigned int, 1>(std::string_view uniformName, const unsigned int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1uiv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<unsigned int, 2>(std::string_view uniformName, const unsigned int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2uiv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<unsigned int, 3>(std::string_view uniformName, const unsigned int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3uiv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<unsigned int, 4>(std::string_view uniformName, const unsigned int* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4uiv(result.value(), count, value);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformV<double, 1>(std::string_view uniformName, const double* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform1dv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<double, 2>(std::string_view uniformName, const double* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform2dv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<double, 3>(std::string_view uniformName, const double* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform3dv(result.value(), count, value);
    }

    template<>
    void Shader::SetUniformV<double, 4>(std::string_view uniformName, const double* value, GLsizei count)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }

        glUniform4dv(result.value(), count, value);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformMat<float, 2>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2fv(result.value(), count, transpose, mat);
    }
    template<>
    void Shader::SetUniformMat<float, 3>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 4>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4fv(result.value(), count, transpose, mat);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformMat<double, 2>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2dv(result.value(), count, transpose, mat);
    }
    template<>
    void Shader::SetUniformMat<double, 3>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 4>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4dv(result.value(), count, transpose, mat);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformMat<float, 2, 3>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2x3fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 3, 2>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3x2fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 2, 4>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2x4fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 4, 2>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4x2fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 3, 4>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3x4fv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<float, 4, 3>(const std::string_view uniformName, const float* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4x3fv(result.value(), count, transpose, mat);
    }

    //-------------------------------------------------------------

    template<>
    void Shader::SetUniformMat<double, 2, 3>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2x3dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 3, 2>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3x2dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 2, 4>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix2x4dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 4, 2>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4x2dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 3, 4>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix3x4dv(result.value(), count, transpose, mat);
    }

    template<>
    void Shader::SetUniformMat<double, 4, 3>(const std::string_view uniformName, const double* mat, GLsizei count, bool transpose)
    {
        auto result = GetUniformLocation(uniformName);
        if(!result.has_value())
        {
            throw UniformNotFoundException(result.error());
            return;
        }
        glUniformMatrix4x3dv(result.value(), count, transpose, mat);
    }
}

//TODO: REWRITE THE ENTIRE FUCKING UNIFORM USING CONSTEXPR IF. And maybe specialize type template to take in glm types later.