#include "Novar/Renderer/Shader.h"
#include "Novar/Renderer/Renderer.h"

#include "Novar/PlatForm/OpenGL/OpenGLShader.h"
#include "Novar/Core/Base.h"
namespace NV
{
    std::shared_ptr<Shader> Shader::Create(const std::string &path)
    {
                switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(path);
        }
        return nullptr;
    }

    std::shared_ptr<Shader> Shader::Create(const std::string& name , const std::string& vertexShader, const std::string& fragmentShader)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                NV_CORE_ASSERT(false,"RendererAPI::None is currently not supported!");
                return nullptr;
                
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name,vertexShader,fragmentShader);
        }
        return nullptr;
    }
    void ShaderLibrary::Add(const std::shared_ptr<Shader> &shader)
    {
        auto& name = shader->GetName();
       
        Add(name, shader);
    }
    void ShaderLibrary::Add(const std::string &name, const std::shared_ptr<Shader> &shader)
    {
        NV_CORE_ASSERT(!Exists(name), "shader  already exitist");
        m_Shaders[name] = shader;

    }
    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }
    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath)
    {
        
        auto shader = Shader::Create(filepath);
        Add(name,shader);
        return shader;
    }
    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string &name)
    {
        NV_CORE_ASSERT(Exists(name), "shader not found");
        return m_Shaders[name];
    }
    bool ShaderLibrary::Exists(const std::string &name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}