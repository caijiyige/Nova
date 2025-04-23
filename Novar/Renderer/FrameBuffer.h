#pragma once 
#include "PCH.h"

namespace NV
{

    struct  FrameBufferSpecification
    {
        uint32_t Width,Height;
        uint32_t Samples = 1;
        //FrameBufferFormat Format = FrameBufferFormat::None;
        bool SwapChainTarget = false;
    };
    class FrameBuffer
    {
        public:
            virtual ~FrameBuffer() = default;
            virtual void Bind() = 0;
            virtual void UnBind() = 0;
            virtual void Resize() = 0;
            virtual uint32_t GetColorAttachmentRendererID() const = 0;
            virtual uint32_t GetDepthAttachmentRendererID() const = 0;


            static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& spec);

            virtual FrameBufferSpecification GetSpecification() const = 0;
    };
}