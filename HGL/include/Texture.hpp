#pragma once

#include "Core.hpp"
#include <filesystem>
#include <exception>
#include <string>
#include <string_view>

namespace HGL
{
    class Texture2D
    {
    public:
        Texture2D();
        Texture2D(const std::filesystem::path& path);
        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&& other);
        ~Texture2D();

        void Bind(uint32_t texSlot = 0) const;
        void SetData(const std::filesystem::path& path);

    private:
        GLuint m_textureID = 0;
        bool m_isLoaded = false;
    };
}