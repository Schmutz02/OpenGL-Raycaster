#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <unordered_map>

namespace Graphics
{
	class Shader
	{

	public:
		static Shader* Default();

	private:
		static Shader* m_Default;

	private:
		struct ShaderProgramSource
		{
			std::string Vertex;
			std::string Fragment;
		};

		unsigned int m_RendererID;
		std::unordered_map<std::string, int> m_LocationCache;

	public:
		Shader(const std::string& filePath);
		Shader(const std::string& vertexShader, const std::string& fragmentShader);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetUniform1i(const std::string& name, int v1);
		void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		int GetUniformLocation(const std::string& name);

		ShaderProgramSource ParseShader(const std::string& path) const;
		unsigned int CompileShader(unsigned int type, const std::string& source) const;
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const;
	};
}

