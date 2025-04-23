#include "PCH.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "OpenGLShader.h"
#include "RendererCommand.h"

namespace NV
{
    struct SquadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float Tiling;
        
    };
    struct Renderer2DData
    {
        static const uint32_t MaxQuardCount = 10000;
        static const uint32_t MaxVertexCount = MaxQuardCount * 4;
        static const uint32_t MaxIndexCount = MaxQuardCount * 6;
        static const uint32_t MaxTextureSlots = 32;

        uint32_t QuardIndexCount = 0;
        SquadVertex* QuardVertexBufferBase = nullptr;
        SquadVertex* QuardVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>,MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;//从1开始，0是白色纹理

        std::shared_ptr<VertexArray> QuardVertexArray;
        std::shared_ptr<VertexBuffer> QuardVertexBuffer;
        std::shared_ptr<Shader> TextureShader;
        std::shared_ptr<Texture> WhiteTexture;

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;

    };
    static Renderer2DData s_Data;



    void Renderer2D::Init()
    {
        
        s_Data.QuardVertexArray = VertexArray::Create();

        
        s_Data.QuardVertexBuffer = VertexBuffer::Create(s_Data.MaxVertexCount *sizeof(SquadVertex));

        s_Data.QuardVertexBuffer->SetLayout(
            {
            {NV::ShaderDataType::Float3, "a_Position",false},
            {NV::ShaderDataType::Float4, "a_Color"   ,false},
            {NV::ShaderDataType::Float2, "a_TexCoord",false},
            {NV::ShaderDataType::Float , "a_TexIndex",false},
            {NV::ShaderDataType::Float , "a_Tiling"  ,false}
            }
        );
        s_Data.QuardVertexArray->AddVertexBuffer(s_Data.QuardVertexBuffer);

        s_Data.QuardVertexBufferBase = new SquadVertex[s_Data.MaxVertexCount];


        uint32_t* quadindices = new uint32_t[s_Data.MaxIndexCount];
        uint32_t offset = 0;
        for(uint32_t i = 0; i < s_Data.MaxIndexCount; i +=6 )
        {
            quadindices[i + 0] = offset + 0;
            quadindices[i + 1] = offset + 1;
            quadindices[i + 2] = offset + 2;
            quadindices[i + 3] = offset + 2;
            quadindices[i + 4] = offset + 3;
            quadindices[i + 5] = offset + 0;

            offset += 4;
        }
        
        //如果是多线程，不要使用后立即删除，因为数据可能排队，删除后却没有上传。最好使用引用计数

        
        std::shared_ptr<IndexBuffer> squdIB = IndexBuffer::Create(quadindices,s_Data.MaxIndexCount);
        s_Data.QuardVertexArray->SetIndexBuffer(squdIB);
        delete[] quadindices;
        
        
        

        s_Data.WhiteTexture = Texture2D::Create(1,1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData,sizeof(uint32_t));

        int samplers[s_Data.MaxTextureSlots];
        for (int i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
    
        s_Data.TextureShader = Shader::Create("F:/LearnGameEngine/Nova/SandBox/assert/shaders/texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniform1iv(std::string("u_Textures"), samplers, static_cast<uint32_t>(s_Data.MaxTextureSlots));

        
        s_Data.TextureSlots[0] = std::static_pointer_cast<Texture2D>(s_Data.WhiteTexture);

        s_Data.QuadVertexPositions[0] = {-0.5f,-0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[1] = { 0.5f,-0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f,0.0f,1.0f};

        
    }

    void Renderer2D::ShutDown()
    {
        
    }

    void Renderer2D::BeginScene(OrthographicCamera& camera)
    {
            
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4f("u_ViewProjection",camera.GetViewProjectionMatrix());


        s_Data.QuardIndexCount = 0;
        s_Data.QuardVertexBufferPtr = s_Data.QuardVertexBufferBase;

        s_Data.TextureSlotIndex = 1;

    }

    void Renderer2D::EndScene()
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuardVertexBufferPtr - (uint8_t*)s_Data.QuardVertexBufferBase);
        s_Data.QuardVertexBuffer->SetData(s_Data.QuardVertexBufferBase,dataSize);

        Flush();
        
    }

    void Renderer2D::Flush()
    {

        s_Data.TextureShader->Bind();
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
    
        RendererCommand::DrawIndexed(s_Data.QuardVertexArray,s_Data.QuardIndexCount);
        s_Data.Stats.DrawCalls++;
        
    }

    void Renderer2D::FlushAndReset()
    {
        EndScene();
        s_Data.QuardVertexBufferPtr = s_Data.QuardVertexBufferBase;
        s_Data.QuardIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::DrawQuard(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        DrawQuard({position.x,position.y,0.0f},size,color);
    }

    void Renderer2D::DrawQuard(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        NV_PROFILE_FUNCTION();

        if (s_Data.QuardIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
        

        const float textureIndex = 0.0f;
        const float Tiling = 1.0f;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});
        

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardIndexCount += 6;
        s_Data.Stats.QuardCount++;

    }

    void Renderer2D::DrawQuard(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture,float Tiling, const glm::vec4& color)
    {
        DrawQuard({position.x,position.y,0.0f},size,texture,Tiling,color);
    }
    void Renderer2D::DrawQuard(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture,float Tiling, const glm::vec4& color)
    {

        NV_PROFILE_FUNCTION();
        if (s_Data.QuardIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
        glm::vec2 TexCoord[4] =
        {
            {0.0f,0.0f},
            {1.0f,0.0f},
            {1.0f,1.0f},
            {0.0f,1.0f}

        };
        
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if(*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        if(textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[0];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[1];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[2];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[3];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardIndexCount += 6;
        s_Data.Stats.QuardCount++;



    }

     void Renderer2D::DrawQuard(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subtexture,float Tiling, const glm::vec4& color)
    {
        DrawQuard({position.x,position.y,0.0f},size,subtexture,Tiling,color);
    }

    void Renderer2D::DrawQuard(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subtexture,float Tiling, const glm::vec4& color)
    {

        NV_PROFILE_FUNCTION();
        if (s_Data.QuardIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
        const std::shared_ptr<Texture2D> texture = subtexture->GetTexture();
    
        const glm::vec2* TexCoord =subtexture->GetTexCoords();

        
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if(*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        if(textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[0];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[1];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[2];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = TexCoord[3];
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardIndexCount += 6;
        s_Data.Stats.QuardCount++;

    }
    void Renderer2D::DrawRotationQuard(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        DrawRotationQuard({position.x,position.y,0.0f},size,rotation,color);
    }
    void Renderer2D::DrawRotationQuard(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {

        const float textureIndex = 0.0f;
        const float Tiling = 1.0f;
        if (s_Data.QuardIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
         glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),glm::vec3(0.0f,0.0f,1.0f))
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardIndexCount += 6;
        s_Data.Stats.QuardCount++;


    }
    void Renderer2D::DrawRotationQuard(const glm::vec2 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture, float Tiling, const glm::vec4& color)
    {
        DrawRotationQuard({position.x,position.y,0.0f},size,rotation,texture,Tiling,color);
    }
    void Renderer2D::DrawRotationQuard(const glm::vec3 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture, float Tiling, const glm::vec4& color)
    {
        if (s_Data.QuardIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if(*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }
        
        if(textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),glm::vec3(0.0f,0.0f,1.0f))
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,0.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {1.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuardVertexBufferPtr->Color = color;
        s_Data.QuardVertexBufferPtr->TexCoord = {0.0f,1.0f};
        s_Data.QuardVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuardVertexBufferPtr->Tiling = Tiling;
        s_Data.QuardVertexBufferPtr++;

        s_Data.QuardIndexCount += 6;
        s_Data.Stats.QuardCount++;

    }
    Renderer2D::Statistics Renderer2D::GetStats()
    { 
        return s_Data.Stats; 
    }
     void Renderer2D::ResetStats()
    { 
        s_Data.Stats.Reset(); 
    }
     

}
