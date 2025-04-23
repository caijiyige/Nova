#include "FrameBuffer.h"

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
            virtual void Resize() override;
            virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
            virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }
        private:
            uint32_t m_FrameBufferID;
            uint32_t m_ColorAttachment;
            uint32_t m_DepthAttachment;
            FrameBufferSpecification m_Specification;
    };
}