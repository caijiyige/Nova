#include "SubTexture2D.h"


namespace NV {
    
    SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
        : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }
    SubTexture2D::~SubTexture2D()
    {
        // Destructor implementation (if needed)
    }

    std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromCoords(const std::shared_ptr<Texture2D> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize)
    {
        glm::vec2 min = { (cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
        glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
        return std::make_shared<SubTexture2D>(texture, min, max);
    }

}


