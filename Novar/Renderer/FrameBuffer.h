#pragma once 

namespace NV
{
    /***********************************
     * 帧缓冲规范，应该包括这个帧缓冲的一些信息，比如宽高，附件规范，是否有交换链
     * 帧缓冲附件规范有很多的纹理规范，使用一个vector管理所有纹理规范，并且此vector可以使用列表初始化
     * 纹理规范就是具体的纹理格式
     * 纹理格式有很多
    ****************************************/
    //帧缓冲 纹理格式
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

    //帧缓冲 纹理规范
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

    //帧缓冲 附件规范
    struct FrameBufferAttachmentSpecification
    {
        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
            : Attachments(attachments)
        {
           
        }
    
        std::vector<FrameBufferTextureSpecification> Attachments;
    };

    //帧缓冲 规范
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
            virtual void Resize(uint32_t width, uint32_t height) = 0;

            virtual int ReadPixelInt(uint32_t attachmentIndex, int x, int y) = 0;
            virtual void ReadPixelColor() = 0;
            virtual void ClearAttachment(uint32_t attachmentIndex, int value) =0;


            virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
            virtual uint32_t GetDepthAttachmentRendererID() const = 0;


            static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

            virtual FrameBufferSpecification GetSpecification() const = 0;
    };
}