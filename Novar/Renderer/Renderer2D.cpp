#include "Novar/PCH.h"
#include "Novar/Renderer/Renderer2D.h"
#include "Novar/Renderer/VertexArray.h"
#include "Novar/Renderer/Shader.h"
#include "Novar/Renderer/RendererCommand.h"
#include "Novar/PlatForm/OpenGL/OpenGLShader.h"
#include "Novar/PlatForm/OpenGL/OpenGLUniformBuffer.h"
#include "Novar/Debug/Instrumentor.h"
#include "Renderer2D.h"

namespace NV
{
    struct SquadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float Tiling;

        // Editor-only
        int EntityID; 
    };
    struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

    struct Renderer2DData
    {
        static const uint32_t MaxQuadCount = 10000;
        static const uint32_t MaxVertexCount = MaxQuadCount * 4;
        static const uint32_t MaxIndexCount = MaxQuadCount * 6;
        static const uint32_t MaxTextureSlots = 32;

        std::shared_ptr<VertexArray> QuadVertexArray;
        std::shared_ptr<VertexBuffer> QuadVertexBuffer;
        std::shared_ptr<Shader> QuadShader;
        std::shared_ptr<Texture> WhiteTexture;

        std::shared_ptr<VertexArray> CircleVertexArray;
        std::shared_ptr<VertexBuffer> CircleVertexBuffer;
        std::shared_ptr<Shader> CircleShader;

        std::shared_ptr<VertexArray> LineVertexArray;
        std::shared_ptr<VertexBuffer> LineVertexBuffer;
        std::shared_ptr<Shader> LineShader;

        uint32_t QuadIndexCount = 0;
        SquadVertex* QuadVertexBufferBase = nullptr;
        SquadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t CircleIndexCount = 0;
        CircleVertex* CircleVertexBufferBase = nullptr;
        CircleVertex* CircleVertexBufferPtr = nullptr;

        uint32_t LineIndexCount = 0;
        LineVertex* LineVertexBufferBase = nullptr;
        LineVertex* LineVertexBufferPtr = nullptr;


        float LineWidth = 2.0f;

        std::array<std::shared_ptr<Texture2D> , MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;//从1开始，0是白色纹理

        glm::vec4 QuadVertexPositions[4];

        struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		std::shared_ptr<UniformBuffer> CameraUniformBuffer;

        Renderer2D::Statistics Stats;

    };
    static Renderer2DData s_Data;



    void Renderer2D::Init()
    {
        //quad
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertexCount *sizeof(SquadVertex));
        s_Data.QuadVertexBuffer->SetLayout(
            {
            {ShaderDataType::Float3, "a_Position",false},
            {ShaderDataType::Float4, "a_Color"   ,false},
            {ShaderDataType::Float2, "a_TexCoord",false},
            {ShaderDataType::Float , "a_TexIndex",false},
            {ShaderDataType::Float , "a_Tiling"  ,false},
            {ShaderDataType::Int   , "a_EntityId",false}
            }
        );
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
        s_Data.QuadVertexBufferBase = new SquadVertex[s_Data.MaxVertexCount];
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
        s_Data.QuadVertexArray->SetIndexBuffer(squdIB);
        delete[] quadindices;

        //Circle
        s_Data.CircleVertexArray = VertexArray::Create();
        s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertexCount *sizeof(CircleVertex));
        s_Data.CircleVertexBuffer->SetLayout(
            {
            {ShaderDataType::Float3, "a_WorldPosition",false},
            {ShaderDataType::Float3, "a_LocalPosition",false},
            {ShaderDataType::Float4, "a_Color"   ,false},
            {ShaderDataType::Float2, "a_Thickness",false},
            {ShaderDataType::Float , "a_Fade",false},
            {ShaderDataType::Int   , "a_EntityId",false}
            }
        );
        s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
        s_Data.CircleVertexArray->SetIndexBuffer(squdIB); // Use quad IB
        s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertexCount];

        //Line
        s_Data.LineVertexArray = VertexArray::Create();
        s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertexCount *sizeof(LineVertex));
        s_Data.LineVertexBuffer->SetLayout(
            {
            {NV::ShaderDataType::Float3, "a_Position",false},
            {NV::ShaderDataType::Float4, "a_Color"   ,false},
            {NV::ShaderDataType::Int   , "a_EntityId",false}
            }
        );
        s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
        s_Data.CircleVertexArray->SetIndexBuffer(squdIB); // Use quad IB
        s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertexCount];

        //white texture
        s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData,sizeof(uint32_t));
        //NV_CORE_WARN("Whitetextrue RendererId:{0}",s_Data.WhiteTexture->GetRendererID());

        int samplers[s_Data.MaxTextureSlots];
        for (int i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;
    
        s_Data.QuadShader   = Shader::Create("F:/LearnGameEngine/Nova/Novar/assert/shaders/quad.glsl");
        s_Data.QuadShader->Bind();
        s_Data.QuadShader->SetUniformBlock("Camera", 0);
        s_Data.QuadShader->SetUniform1iv("u_Textures", samplers, s_Data.MaxTextureSlots);
        s_Data.QuadShader->Unbind();

        s_Data.CircleShader = Shader::Create("F:/LearnGameEngine/Nova/Novar/assert/shaders/circle.glsl");
        s_Data.CircleShader->Bind();
        s_Data.CircleShader->SetUniformBlock("Camera", 0);
        s_Data.CircleShader->Unbind();

        s_Data.LineShader = Shader::Create("F:/LearnGameEngine/Nova/Novar/assert/shaders/line.glsl");
        s_Data.LineShader->Bind();
        s_Data.LineShader->SetUniformBlock("Camera", 0);
        s_Data.LineShader->Unbind();

        s_Data.TextureSlots[0] = std::static_pointer_cast<Texture2D>(s_Data.WhiteTexture);

        s_Data.QuadVertexPositions[0] = {-0.5f,-0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[1] = { 0.5f,-0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f,0.0f,1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f,0.0f,1.0f};

        

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
        
    }

    void Renderer2D::ShutDown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera, const glm::mat4 &transform)
    {
        s_Data.CameraBuffer.ViewProjection = camera->GetProjectionMatrix() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
        StartBatch();
    }

    void Renderer2D::BeginScene(OrthographicCamera& camera)
    {
        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
        StartBatch();
    }

    void Renderer2D::BeginScene(const std::shared_ptr<EditorCamera> &camera)
    {
        s_Data.CameraBuffer.ViewProjection = camera->GetProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineIndexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;	
		/*
		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;
*/
		s_Data.TextureSlotIndex = 1;
	}


    void Renderer2D::Flush()
    {

        if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
            // Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);
                //NV_CORE_WARN("texture slot:{0} Renderid:{1}",i,s_Data.TextureSlots[0]->GetRendererID());
            }
			s_Data.QuadShader->Bind();
            
			RendererCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}
        if(s_Data.CircleIndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);
			s_Data.CircleShader->Bind();
			RendererCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
        }

        if(s_Data.LineIndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
			s_Data.LineShader->Bind();
            RendererCommand::SetLineWidth(s_Data.LineWidth);
			RendererCommand::DrawLine(s_Data.LineVertexArray, s_Data.LineIndexCount);
			s_Data.Stats.DrawCalls++;
        }

        
    }

    void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

    void Renderer2D::FlushAndReset()
    {
        EndScene();
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		NV_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

        //NV_CORE_WARN("color {0}, {1}, {2}, {3}",color.x,color.y,color.z,color.w);
        //NV_CORE_WARN("entityID {0}",entityID);

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndexCount)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->Tiling = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
        //NV_CORE_WARN("posion {0}, {1}, {2}",s_Data.QuadVertexBufferPtr->Position.x,s_Data.QuadVertexBufferPtr->Position.y,s_Data.QuadVertexBufferPtr->Position.z);

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		

		// TODO: implement for circles
		// if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

    void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineIndexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

    void Renderer2D::DrawSprite(const glm::mat4 &transform, SpriteRendererComponent &src, int entityID)
    {
        if (src.Texture)
			DrawQuad(transform, src.Texture, src.Tiling, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
    }
    void Renderer2D::DrawQuad(const glm::mat4 &transform, const std::shared_ptr<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor, int entityID)
    {
		NV_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndexCount)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

        //NV_CORE_WARN("color {0}, {1}, {2}, {3}",tintColor.x,tintColor.y,tintColor.z,tintColor.w);
        //NV_CORE_WARN("tiling {0}",tilingFactor);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->Tiling = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
        NV_CORE_WARN("posion {0}, {1}, {2}",s_Data.QuadVertexBufferPtr->Position.x,s_Data.QuadVertexBufferPtr->Position.y,s_Data.QuadVertexBufferPtr->Position.z);

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        DrawQuad({position.x,position.y,0.0f},size,color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        NV_PROFILE_FUNCTION();


        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});
        
        DrawQuad(transform,color);

    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture,float Tiling, const glm::vec4& color)
    {
        DrawQuad({position.x,position.y,0.0f},size,texture,Tiling,color);
    }
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture,float Tiling, const glm::vec4& color)
    {

        NV_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        DrawQuad(transform,texture,Tiling,color);


    }

     void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subtexture,float Tiling, const glm::vec4& color)
    {
        DrawQuad({position.x,position.y,0.0f},size,subtexture,Tiling,color);
    }

    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subtexture,float Tiling, const glm::vec4& color)
    {

        NV_PROFILE_FUNCTION();
        if (s_Data.QuadIndexCount > Renderer2DData::MaxIndexCount)
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

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[0];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[1];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[2];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = TexCoord[3];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;

    }
    
    void Renderer2D::DrawRotationQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        DrawRotationQuad({position.x,position.y,0.0f},size,rotation,color);
    }
    void Renderer2D::DrawRotationQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {

        const float textureIndex = 0.0f;
        const float Tiling = 1.0f;
        if (s_Data.QuadIndexCount > Renderer2DData::MaxIndexCount)
            FlushAndReset();
         glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) 
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotation),glm::vec3(0.0f,0.0f,1.0f))
        * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,0.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {1.0f,0.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {1.0f,1.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,1.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;


    }
    void Renderer2D::DrawRotationQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture, float Tiling, const glm::vec4& color)
    {
        DrawRotationQuad({position.x,position.y,0.0f},size,rotation,texture,Tiling,color);
    }
    void Renderer2D::DrawRotationQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture, float Tiling, const glm::vec4& color)
    {
        if (s_Data.QuadIndexCount > Renderer2DData::MaxIndexCount)
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

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,0.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {1.0f,0.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {1.0f,1.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,1.0f};
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->Tiling = Tiling;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;

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
