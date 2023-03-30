#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

namespace Graphics
{
    const char* defaultVertex = "#version 330 core\n"
        "layout (location = 0) in vec4 aPos;\n"
        "layout (location = 1) in vec4 aCol;\n"
        "out vec4 Color;\n"
		"uniform mat4 u_MVP;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = u_MVP * aPos;\n"
        "   Color = aCol;\n"
        "}\0";
    const char* defaultFrag = "#version 330 core\n"
        "in vec4 Color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = Color;\n"
        "}\n\0";

    Shader* Shader::m_Default = nullptr;
    Shader* Shader::Default()
    {
        if (m_Default)
            return m_Default;

        m_Default = new Shader(defaultVertex, defaultFrag);
        return m_Default;
    }

    Shader::Shader(const std::string& filePath)
    {
        ShaderProgramSource shaderSource = ParseShader(filePath);
        m_RendererID = CreateShader(shaderSource.Vertex, shaderSource.Fragment);
    }

    Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        m_RendererID = CreateShader(vertexShader, fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::UnBind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniform1i(const std::string& name, int v1)
    {
        glUniform1i(GetUniformLocation(name), v1);
    }

    void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
    {
        glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
    }

    void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    int Shader::GetUniformLocation(const std::string& name)
    {
        if (m_LocationCache.find(name) != m_LocationCache.end())
            return m_LocationCache[name];

        int location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

        m_LocationCache[name] = location;
        return location;
    }

    Shader::ShaderProgramSource Shader::ParseShader(const std::string& path) const
    {
        std::ifstream stream(path);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1, LENGTH
        };

        std::string line;
        std::stringstream ss[(int)ShaderType::LENGTH];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                if (type == ShaderType::NONE)
                    continue;

                ss[(int)type] << line << '\n';
            }
        }

        return ShaderProgramSource { ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str() };
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string& source) const
    {
        const unsigned int id = glCreateShader(type);
        const char* src = source.c_str();

        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);

            const char* typeString = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
            std::cout << "Failed to compile " << typeString << " shader!" << std::endl;
            std::cout << message << std::endl;

            glDeleteShader(id);

            return 0;
        }

        return id;
    }

    unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
}
