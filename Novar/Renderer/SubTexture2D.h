#pragma once
#include "PCH.h"
#include "Texture.h"
#include "glm.hpp"
namespace NV
{
    class SubTexture2D
    {
   
        
    public:
        SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
        ~SubTexture2D();
        static std::shared_ptr<SubTexture2D> CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.0f, 1.0f});


        inline const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }
        inline const glm::vec2* GetTexCoords() const { return m_TexCoords; }

        private:
        std::shared_ptr<Texture2D> m_Texture;
        

        // Texture coordinates for the four corners of the subtexture
        // These coordinates are used for texture mapping


        glm::vec2 m_TexCoords[4];

    };
    
}