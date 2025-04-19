#pragma once
#include "Texture.h" 
namespace NV
{
    class OpenGLTexture2D : public Texture2D
    {

        public:
            OpenGLTexture2D(const std::string& path);
            virtual ~OpenGLTexture2D();

            virtual unsigned int GetWidth() const override { return m_Width;};
            virtual unsigned int GetHeight() const override { return m_Height;};

            virtual void Bind(unsigned int slot = 0) const override;
            virtual void UnBind() const override;

        private:

            std::string m_path;
            unsigned int m_Width;
            unsigned int m_Height;
            unsigned int m_RedererID;
    };
}