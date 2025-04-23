#include "Buffer.h"

namespace NV
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
        public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices,uint32_t size);

        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
        virtual void SetData(const void* data,uint32_t size) override ;
        virtual const BufferLayout& GetLayout() const override { return m_Layout;} ;

        private:
        unsigned int m_RenderId;
        BufferLayout m_Layout;

    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
        public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual unsigned int GetCount() const override{return m_Count;}

        private:

        unsigned int m_RenderId;
        unsigned int m_Count;

    };
}