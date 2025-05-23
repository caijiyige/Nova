#pragma once
#include <string>
#include "glm.hpp"
namespace NV
{
    class Shader
    {
    public:
        
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string& GetName() const =0;

        static std::shared_ptr<Shader> Create(const std::string& path);
         
        static std::shared_ptr<Shader> Create(const std::string& name,const std::string& vertexShader, const std::string& fragmentShader);

    private:
        unsigned int m_RendererID;// OpenGL shader program ID
       
    };

    class ShaderLibrary
    {
        public:
        void Add(const std::shared_ptr<Shader>& shader);
        void Add(const std::string& name,const std::shared_ptr<Shader>& shader);
        std::shared_ptr<Shader> Load(const std::string& filepath);
        std::shared_ptr<Shader> Load(const std::string& name,const std::string& filepath);

        std::shared_ptr<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;
        private:
        std::unordered_map<std::string,std::shared_ptr<Shader>> m_Shaders;

    };
}