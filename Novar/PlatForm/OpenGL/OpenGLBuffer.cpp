#include "Novar/PlatForm/OpenGL/OpenGLBuffer.h"
#include <glad.h>
namespace NV{
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        glCreateBuffers(1,&m_RenderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
        glBufferData(GL_ARRAY_BUFFER,size,nullptr,GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices,uint32_t size)
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

    void OpenGLVertexBuffer::SetData(const void *data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    //////////////////////////////////////////////////////////////////////////////
    ////IndexBuffer///////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		

		glCreateBuffers(1, &m_RenderId);
		
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
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