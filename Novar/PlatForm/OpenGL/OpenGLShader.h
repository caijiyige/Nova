#pragma once
#include "Shader.h"

namespace NV
{
    class OpenGLShader: public Shader
    {
    public:
        OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& path);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        const std::string& GetName() const override{return m_Name;}

        void SetUniform1f(const std::string& name, float value) ;
        void SetUniform1i(const std::string& name, int value);
        void SetUniform2f(const std::string& name, const glm::vec2& vector);
        void SetUniform3f(const std::string& name, const glm::vec3& vector);
        void SetUniform4f(const std::string& name, const glm::vec4& vector);
        void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);
        void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    private:
        unsigned int m_RendererID;// OpenGL shader program ID
        std::string m_Name;// Shader name


        std::string ReadFile(const std::string& path);
        std::unordered_map<unsigned int,std::string> Preprocess(const std::string& source);
        //std::unordered_map<unsigned int,std::string> Preprocess(const std::string& path);

        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    };
}