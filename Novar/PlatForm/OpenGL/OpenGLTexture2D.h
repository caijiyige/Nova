#pragma once
#include "Novar/Renderer/Texture.h"
#include <glad.h>
namespace NV
{
    class OpenGLTexture2D : public Texture2D
    {

        public:
            OpenGLTexture2D(const std::string& path);
            OpenGLTexture2D(const uint32_t width,const uint32_t height);
            OpenGLTexture2D(const TextureSpecification& specification);
            virtual ~OpenGLTexture2D();
            
            virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }
            virtual unsigned int GetWidth() const override { return m_Width;}
            virtual unsigned int GetHeight() const override { return m_Height;}

            virtual unsigned int GetRendererID() const override { return m_RedererID;}

            virtual void SetData(void* data,uint32_t size) override;
            virtual const std::string& GetPath() const override { return m_Path; }

            virtual void Bind(unsigned int slot = 0) const override;
            virtual void UnBind() const override;

            virtual bool IsLoaded() const override{ return m_IsLoaded; };

            virtual bool operator==(const Texture& other) const override
            {
                return m_RedererID == ((OpenGLTexture2D&)other).m_RedererID;
            }
        private:
            TextureSpecification m_Specification;

            std::string m_Path;
            bool m_IsLoaded = false;
            unsigned int m_Width;
            unsigned int m_Height;
            unsigned int m_RedererID;
		    GLenum m_InternalFormat, m_DataFormat;

    };
}