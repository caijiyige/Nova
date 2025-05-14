#include "Novar/PlatForm/OpenGL/OpenGLVertexArray.h"
#include <glad.h>
namespace NV
{

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
        }
    }
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1,&m_RendererId);
    }
    OpenGLVertexArray::~OpenGLVertexArray()
    {

    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);

    }
    void OpenGLVertexArray::UnBind() const
    {
        glBindVertexArray(0);

    }
    void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> &vertexBuffer)
    {
        glBindVertexArray(m_RendererId);
        vertexBuffer->Bind();
        
        const auto& layouts  = vertexBuffer->GetLayout();

        unsigned int index = 0;
        for (const auto& element : layouts)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetElementsCount(), 
                ShaderDataTypeToOpenGLBaseType(element.Type), 
                element.Normalized ? GL_TRUE : GL_FALSE,
                layouts.GetStride(),
                (const void*)element.Offset);
            index++;
            
        }
        m_VertexBuffers.push_back(vertexBuffer);

    }
    void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> &indexBuffer)
    {
        glBindVertexArray(m_RendererId);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;

    }
}