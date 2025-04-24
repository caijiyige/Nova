#include "OpenGLFrameBuffer.h"
#include "glad.h"
namespace NV
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification &spec)
    : m_Specification(spec)
    {
        NV_PROFILE_FUNCTION();
        
        Invalidate();

    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        NV_PROFILE_FUNCTION();
        glDeleteFramebuffers(1, &m_FrameBufferID);
        glDeleteTextures(1, &m_ColorAttachment);
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
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }


        glGenFramebuffers(1, &m_FrameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
      /*
        glGenTextures(1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8, nullptr);
        //glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
        glGenRenderbuffers(1, &m_DepthAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
        
        NV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::Resize(float x,float y)
    {
        m_Specification.Width = x;
        m_Specification.Height = y;
        Invalidate();
    }
        
}