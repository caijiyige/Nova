#pragma once
#include "Novar/Renderer/FrameBuffer.h"
#include "Novar/Core/Base.h"
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

            virtual void ReadPixelColor() override;
            virtual void Resize(uint32_t width, uint32_t height) override;

            virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

            void Invalidate() ;
            virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {  return m_ColorAttachments.at(index); }
            virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
        private:
            uint32_t m_FrameBufferID = 0;
            
            uint32_t m_DepthAttachment = 0;

            FrameBufferSpecification m_Specification;
            std::vector<uint32_t> m_ColorAttachments;

            std::vector<FrameBufferTextureSpecification> m_ColorAttachmentsSpecification;

            FrameBufferTextureSpecification m_DepthAttachmentSpecification;

            std::vector<FrameBufferTextureSpecification> m_StencilAttachments;
            
            FrameBufferTextureFormat m_DepthFormat = FrameBufferTextureFormat::None;

    };
}