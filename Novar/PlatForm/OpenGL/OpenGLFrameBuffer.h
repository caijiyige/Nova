#pragma once
#include "Novar/Renderer/FrameBuffer.h"
namespace NV
{
 
    class OpenGLFrameBuffer : public FrameBuffer
    {
        public:
            OpenGLFrameBuffer(const FrameBufferSpecification& spec);

            virtual FrameBufferSpecification GetSpecification() const override { return m_Specification; }

            virtual ~OpenGLFrameBuffer();
            virtual void Bind() override;
            virtual void UnBind() override;

            virtual int ReadPixelInt(uint32_t attachmentIndex, int x, int y) override;
            virtual void Resize(float x,float y) override;

            virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

            void Invalidate() ;
            virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
            virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
        private:
            uint32_t m_FrameBufferID = 0;
            uint32_t m_ColorAttachment = 0;
            uint32_t m_DepthAttachment = 0;
            std::vector<uint32_t> m_ColorAttachments;
            FrameBufferSpecification m_Specification;

            std::vector<FrameBufferTextureSpecification> m_ColorAttachmentsSpecification;

            FrameBufferTextureSpecification m_DepthAttachmentSpecification;

            std::vector<FrameBufferTextureSpecification> m_StencilAttachments;
            
            FrameBufferTextureFormat m_DepthFormat = FrameBufferTextureFormat::None;

    };
}