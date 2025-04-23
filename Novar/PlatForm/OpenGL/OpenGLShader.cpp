#include "PCH.h"
#include <fstream>
#include "OpenGLShader.h"
#include "Log.h"
#include "glad.h"



namespace NV
{
    static unsigned int ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
        NV_CORE_ASSERT(false, "Unknown type");
    }
    OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
        m_RendererID = CreateShader(vertexSrc, fragmentSrc);
    }
    OpenGLShader::OpenGLShader(const std::string &path)
    {
        std::string source = ReadFile(path);
        auto shaderSource = Preprocess(source);
        m_RendererID = CreateShader(shaderSource[GL_VERTEX_SHADER],shaderSource[GL_FRAGMENT_SHADER]);

        auto lastSlush = path.find_last_of("/\\");
        lastSlush = lastSlush==std::string::npos ? 0 : lastSlush + 1;

        auto lastDot = path.rfind("."); 
        auto count = lastSlush==std::string::npos ? path.size() - lastSlush: lastDot - lastSlush;
        m_Name = path.substr(lastSlush,count);

    }
    
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }
    void OpenGLShader::Bind() const 
    {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }
    void OpenGLShader::SetUniform1f(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetUniform1i(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
    void OpenGLShader::SetUniform1iv(const std::string &name, int *values,uint32_t count)
    {
        glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, values);
    }
    void OpenGLShader::SetUniform2f(const std::string &name, const glm::vec2 &vector)
    {
        glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), vector.x, vector.y);
    }
    void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& vector)
    {
        glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), vector.x, vector.y, vector.z);
    }
    void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& vector)
    {
        glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), vector.x, vector.y, vector.z, vector.w);
    }
    void OpenGLShader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }
    void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }
    std::string OpenGLShader::ReadFile(const std::string& path)
    {
        std::string result;
        std::ifstream in(path,std::ios::in |std::ios::binary);

        if (in)
        {
           in.seekg(0,std::ios::end);
           result.resize(in.tellg());

           in.seekg(0,std::ios::beg);
           in.read(&result[0],result.size());
           in.close();
        }
        else
        NV_CORE_ASSERT("Could not open file '{0}'",path);
        return result;
    }
    std::unordered_map<unsigned int, std::string> OpenGLShader::Preprocess(const std::string &source)
    {
        std::unordered_map<unsigned int, std::string> shaderSource;
        
        const char* typeToken = "#type";
        size_t typeTokenlength = strlen(typeToken);

        size_t pos = source.find(typeToken,0);

        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			NV_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenlength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			NV_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			NV_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
            if (nextLinePos == std::string::npos) {
                break;
            }
			shaderSource[ShaderTypeFromString(type)] = (nextLinePos == std::string::npos) ? source.substr(nextLinePos,pos - source.size() - 1) : source.substr(nextLinePos, pos - nextLinePos);
        }
        return shaderSource;

    }

    unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string &source)
    {
        unsigned int id = glCreateShader(type);// create shader object
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);// set the source code
        glCompileShader(id);// compile the shader

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);// check for compile errors
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            NV_CORE_ERROR("Failed to compile {0} shader!", (type == GL_VERTEX_SHADER) ? "vertex" : "fragment");
            NV_CORE_ERROR("{0}", message);
            glDeleteShader(id);
            return 0;
        }
        return id;
    }
    unsigned int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);// compile vertex shader
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);// compile fragment shader

        glAttachShader(program, vs);// attach vertex shader
        glAttachShader(program, fs);// attach fragment shader
        glLinkProgram(program);     // link the program
        glValidateProgram(program);// validate the program

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
}