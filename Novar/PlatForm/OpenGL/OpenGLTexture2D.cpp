#include "Novar/PlatForm/OpenGL/OpenGLTexture2D.h"

#include "Novar/Core/Log.h"

#include <glad.h>
#include <stb_image.h>


namespace NV{

    void GetFormat(int iChannels, GLenum& uiInternalFormat, GLenum& uiDataFormat)
	{
		switch (iChannels)
		{
		case 1:
		{
			uiInternalFormat = GL_RED;
			uiDataFormat = GL_RED;
			break;
		}
		case 3:
		{
			uiInternalFormat = GL_RGB8;
			uiDataFormat =  GL_RGB;
			break;
		}
		case 4:
		{
			uiInternalFormat = GL_RGBA8;
			uiDataFormat = GL_RGBA;
			break;
		}
		default:
		{
			NV_CORE_ERROR("Unkown Image Data Channel");
			break;
		}
		}
	}

    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
    :m_Width(width),m_Height(height)
    {

        GLenum internalFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RedererID);

        glTextureStorage2D(m_RedererID, 1 ,internalFormat,m_Width,m_Height);

        glTextureParameteri(m_RedererID, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTextureParameteri(m_RedererID, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTextureParameteri(m_RedererID,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTextureParameteri(m_RedererID,GL_TEXTURE_WRAP_T,GL_REPEAT);

    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
    :m_Path(path)
    {
        int width,height,channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data =  stbi_load(m_Path.c_str(),&width,&height,&channels,0);
        NV_CORE_ASSERT(data," failed to load image");
        m_IsLoaded = true;
        m_Width = width;
        m_Height = height;
        GLenum internalFormat = GL_RGB8;
        GLenum dataFormat = GL_RGB;

        GetFormat(channels,internalFormat,dataFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RedererID);

        glTextureStorage2D(m_RedererID, 1 ,internalFormat,m_Width,m_Height);

        glTextureParameteri(m_RedererID, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTextureParameteri(m_RedererID, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glTextureParameteri(m_RedererID,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTextureParameteri(m_RedererID,GL_TEXTURE_WRAP_T,GL_REPEAT);

        glTextureSubImage2D(m_RedererID, 0 ,0, 0 ,m_Width,m_Height,dataFormat,GL_UNSIGNED_BYTE,data);

        stbi_image_free(data);

    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1,&m_RedererID);

    }

    void OpenGLTexture2D::SetData(void *data, uint32_t size)
    {
        NV_CORE_ASSERT(size == m_Width * m_Height *4 , "data must be entire texture");
        glTextureSubImage2D(m_RedererID, 0 ,0, 0 ,m_Width,m_Height,GL_RGBA,GL_UNSIGNED_BYTE,data);
    }

    void OpenGLTexture2D::Bind(unsigned int slot) const
    {
        
        glActiveTexture(GL_TEXTURE0 + slot);
	    glBindTexture(GL_TEXTURE_2D, m_RedererID);

    }
    void OpenGLTexture2D::UnBind() const
    {
        glBindTexture(GL_TEXTURE_2D,0);
    }
}