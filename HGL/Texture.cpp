#include "include/Texture.hpp"
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace HGL
{
    Texture2D::Texture2D()
    {
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        m_isLoaded = false;
    }

    Texture2D::Texture2D(const std::filesystem::path& path)
    {
        int width, height, numChannels;

        stbi_set_flip_vertically_on_load(true);
        //for some reason, std::filesystem::path::c_str() returns a const wchar_t* instead of const char*
        std::unique_ptr<uint8_t, void(*)(uint8_t*)> imageData(
            stbi_load(path.string().c_str(), &width, &height, &numChannels, 4), 
            [](uint8_t* data) {stbi_image_free(static_cast<void*>(data));}
        );

        if (imageData == nullptr)
        {
            m_isLoaded = false;
            return;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<const void*>(imageData.get()));

        glGenerateMipmap(GL_TEXTURE_2D);
        
                                                                      
        m_isLoaded = true;
    }

    Texture2D::Texture2D(Texture2D&& other)
    {
        this->m_textureID = other.m_textureID;
        this->m_isLoaded = other.m_isLoaded;

        other.m_textureID = 0;
        other.m_isLoaded = false;
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
        m_isLoaded = false;
    }

    void Texture2D::Bind(uint32_t texSlot) const 
    {

        if(!m_isLoaded)
        {
            throw InvalidGLObjectBind("Tried to bind invalid texture");
            return;
        }

        glActiveTexture(GL_TEXTURE0 + texSlot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture2D::SetData(const std::filesystem::path& path)
    {
        //TODO: 
        (void) path;
    }
}