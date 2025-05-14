#include "Novar/PlatForm/OpenGL/OpenGLFrameBuffer.h"
#include "Novar/Core/Base.h"
#include "Novar/Debug/Instrumentor.h"
#include <glad.h>
namespace NV
{
    static const uint32_t s_MaxFrameBufferSize = 8192;
    static const uint32_t s_MaxFrameBufferSamples = 4;
    static const uint32_t s_MaxFrameBufferColorAttachments = 4;
    static const uint32_t s_MaxFrameBufferDepthAttachments = 1;
    static const uint32_t s_MaxFrameBufferStencilAttachments = 1;

    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }
        static void BindTexture( bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }
        static void AttachColorTexture(uint32_t id, int samples,GLenum internalFormat ,GLenum format, uint32_t attachment, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, TextureTarget(multisampled), id, 0);
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_TRUE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format,uint32_t attachment, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, TextureTarget(multisampled), id, 0);
            if (multisampled)
            {
                glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_TRUE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 0, format, width, height);
                

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, TextureTarget(multisampled), id, 0);
        }


        static void CreatTextures(bool multisampled, uint32_t count, uint32_t* outIDs)
        {
            
            glCreateTextures(TextureTarget(multisampled), count, outIDs);
            for (uint32_t i = 0; i < count; i++)
            {
                glBindTexture(GL_TEXTURE_2D, outIDs[i]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
        }
        static bool IsDepthFormat(FrameBufferTextureFormat format)
        {
            switch (format)
            {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                case FrameBufferTextureFormat::Depth:
                case FrameBufferTextureFormat::Stencil:
                    return true;
            }
            return false;
        }
        static GLenum TextureFormat(FrameBufferTextureFormat format)
        {
            switch (format)
            {
                case FrameBufferTextureFormat::RGBA8:
                    return GL_RGBA8;
                case FrameBufferTextureFormat::RED_INTEGER:
                    return GL_R32I;
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    return GL_DEPTH24_STENCIL8;
                default:
                    NV_CORE_ASSERT(false, "Unknown FrameBufferTextureFormat!");
                    return 0;
            }
        }
    }

    

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification &spec)
    : m_Specification(spec)
    {
        NV_PROFILE_FUNCTION();
        for(auto attachment : m_Specification.AttachmentSpec.Attachments)
        {
           if (Utils::IsDepthFormat(attachment.TextureFormat))
            {
                m_DepthAttachmentSpecification = attachment;
                m_DepthFormat = attachment.TextureFormat;
            }
            else
            {
                m_ColorAttachmentsSpecification.push_back(attachment);
            }
        }
        
        
        Invalidate();

    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        NV_PROFILE_FUNCTION();
        glDeleteFramebuffers(1, &m_FrameBufferID);
        glDeleteTextures(m_ColorAttachments.size(),m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }
    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);

        
    }
    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::Invalidate()
    {

         NV_PROFILE_FUNCTION();
        if (m_FrameBufferID)
        {
            glDeleteFramebuffers(1, &m_FrameBufferID);
            glDeleteTextures(m_ColorAttachments.size(),m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
            
            m_DepthAttachment = 0;
            m_ColorAttachments.clear();
            
        }


        glGenFramebuffers(1, &m_FrameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        //Attachments
        if(m_Specification.SwapChainTarget)
        {
            m_ColorAttachment = 0;
            m_DepthAttachment = 0;
            return;
        }
        bool multisampled = m_Specification.Samples > 1;
        //Color Attachment
        if(m_ColorAttachmentsSpecification.size())
        {
           m_ColorAttachments.resize(m_ColorAttachmentsSpecification.size());
           Utils::CreatTextures(multisampled, m_ColorAttachments.size(), m_ColorAttachments.data());
              for (size_t i = 0; i < m_ColorAttachments.size(); i++)
              {
                Utils::BindTexture(multisampled, m_ColorAttachments[i]);
                switch (m_ColorAttachmentsSpecification[i].TextureFormat)
                {
                    case FrameBufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_ColorAttachments[i],m_Specification.Samples,GL_RGBA8,GL_RGBA,GL_COLOR_ATTACHMENT0 + i, m_Specification.Width, m_Specification.Height);
                        break;
                    case FrameBufferTextureFormat::RED_INTEGER:
                        Utils::AttachColorTexture(m_ColorAttachments[i],m_Specification.Samples,GL_R32I,GL_RED_INTEGER,GL_COLOR_ATTACHMENT0 + i, m_Specification.Width, m_Specification.Height);
                        break;

                    default:
                        NV_CORE_ASSERT(false,"Unknown FrameBufferTextureFormat!");
                        break;
                }
              }
        }

        //Depth Attachment
        if(m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
        {
            m_DepthAttachment = 0;
            Utils::CreatTextures(multisampled, 1, &m_DepthAttachment);
            Utils::BindTexture(multisampled, m_DepthAttachment);
            switch (m_DepthFormat)
            {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(multisampled, m_Specification.Samples, GL_DEPTH24_STENCIL8 , GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                    break;
            }
        }
        else
        {
            m_DepthAttachment = 0;
        }
        //Stencil Attachment


        if(m_ColorAttachments.size() > 1)
        {
            GLenum buffers[s_MaxFrameBufferColorAttachments];
            glDrawBuffers(m_ColorAttachments.size(), buffers);

        }
       
        NV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int OpenGLFrameBuffer::ReadPixelInt(uint32_t attachmentIndex, int x, int y)
    {
        NV_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelValue = 0;
        glReadnPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, sizeof(int), &pixelValue);
        return pixelValue;
    }
    void OpenGLFrameBuffer::Resize(float x, float y)
    {
        m_Specification.Width = x;
        m_Specification.Height = y;
        Invalidate();
    }

    void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        NV_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range!");
        auto& attachment = m_ColorAttachmentsSpecification[attachmentIndex];

        glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::TextureFormat(attachment.TextureFormat), GL_UNSIGNED_BYTE, &value);
        

    }
}