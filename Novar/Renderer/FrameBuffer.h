#pragma once 

namespace NV
{
    enum class FrameBufferTextureFormat
    {
        None = 0,
        //Color
        RGBA8 = 1,
        RED_INTEGER = 2,

        //Depth
        DEPTH24STENCIL8 = 3,
        DEPTH32FSTENCIL8 = 4,
        Depth = 5,
        //Stencil
        Stencil = 6,
    };
    struct FrameBufferTextureSpecification
    {
        FrameBufferTextureFormat TextureFormat;
        FrameBufferTextureSpecification()
        {
            TextureFormat = FrameBufferTextureFormat::RGBA8;
        }
        FrameBufferTextureSpecification(FrameBufferTextureFormat format)
            : TextureFormat(format)
        {
        }
    };

    struct FrameBufferAttachmentSpecification
    {
        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
            : Attachments(attachments)
        {
           
        }
    
        std::vector<FrameBufferTextureSpecification> Attachments;
    };

    struct  FrameBufferSpecification
    {
        uint32_t Width,Height;
        uint32_t Samples = 1;
        FrameBufferAttachmentSpecification AttachmentSpec;
        bool SwapChainTarget = false;
    };
    class FrameBuffer
    {
        public:
            virtual ~FrameBuffer() = default;
            virtual void Bind() = 0;
            virtual void UnBind() = 0;
            virtual void Resize(float x,float y) = 0;

            virtual int ReadPixelInt(uint32_t attachmentIndex, int x, int y) = 0;

            virtual void ClearAttachment(uint32_t attachmentIndex, int value) =0;


            virtual uint32_t GetColorAttachmentRendererID() const = 0;
            virtual uint32_t GetDepthAttachmentRendererID() const = 0;


            static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

            virtual FrameBufferSpecification GetSpecification() const = 0;
    };
}