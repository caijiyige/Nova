#pragma once
#include "PCH.h"
#include "Log.h"
namespace NV{

    enum class ShaderDataType
    {
        None = 0 , Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static unsigned int ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:  return sizeof(float);
            case ShaderDataType::Float2: return sizeof(float)*2;
            case ShaderDataType::Float3: return sizeof(float)*3;
            case ShaderDataType::Float4: return sizeof(float)*4;
            case ShaderDataType::Mat3:   return sizeof(float)*3*3;
            case ShaderDataType::Mat4:   return sizeof(float)*4*4;
            case ShaderDataType::Int:    return sizeof(int);
            case ShaderDataType::Int2:   return sizeof(int)*2;
            case ShaderDataType::Int3:   return sizeof(int)*3;
            case ShaderDataType::Int4:   return sizeof(int)*4;
            case ShaderDataType::Bool:   return sizeof(bool);
            
        }
        
        return 0;
    }


    struct BufferElement
    {
        ShaderDataType Type;
        std::string Name;

        unsigned int Size;
        unsigned int Offset;
        bool Normalized;

        BufferElement(ShaderDataType type,const std::string& name,bool normalized = false)
        :Type(type),Name(name),Size(ShaderDataTypeSize(type)),Offset(0),Normalized(normalized)
        {

        }

        unsigned int GetElementsCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 3*3;
            case ShaderDataType::Mat4:   return 4*4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            }
        NV_CORE_ASSERT(false,"Unknown ElementsCount")
        return 0;
        }

    };

    class BufferLayout
    {
        public:
            BufferLayout() = default;
            BufferLayout(const std::initializer_list<BufferElement>& elements)
            :m_Elements(elements)
            {
                CalculateOffsetAndStride(); 
            }
            inline const std::vector<BufferElement>& GetElements() const {return m_Elements;}

            std::vector<BufferElement>::iterator begin() {return m_Elements.begin();}
            std::vector<BufferElement>::iterator end() {return m_Elements.end();}
            std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

            inline unsigned int GetStride () const {return m_Stride;};

        private:
            std::vector<BufferElement> m_Elements;
            unsigned int m_Stride = 0;

            void CalculateOffsetAndStride(){
                unsigned int offset = 0;
                m_Stride = 0;


                for (auto& element :m_Elements)
                {
                    element.Offset = offset;
                    offset += element.Size;
                    m_Stride += element.Size;
                }
            }
        
    };





    class VertexBuffer
    {
    private:
        /* data */
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static VertexBuffer* Create(float* vertices, unsigned int size);
    };
    

    class IndexBuffer
    {

    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual unsigned int GetCount() const = 0;

        static IndexBuffer* Create(unsigned int* indices, unsigned int count);
    };
    
}