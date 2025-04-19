#include "OpenGLBuffer.h"
#include "glad.h"
namespace NV{

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices,unsigned int size)
    {
        glCreateBuffers(1,&m_RenderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
        glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
    }
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1,&m_RenderId);
    }
    void OpenGLVertexBuffer::Bind() const
    {
        glBindVertexArray(m_RenderId);

    }
    void OpenGLVertexBuffer::UnBind() const
    {
        glBindVertexArray(0);

    }



    //////////////////////////////////////////////////////////////////////////////
    ////IndexBuffer///////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices,unsigned int count)
    :m_Count(count)
    {
        glCreateBuffers(1,&m_RenderId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(unsigned int),indices,GL_STATIC_DRAW);
    }
    
    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1,&m_RenderId);
    }
    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
    }
    void OpenGLIndexBuffer::UnBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}