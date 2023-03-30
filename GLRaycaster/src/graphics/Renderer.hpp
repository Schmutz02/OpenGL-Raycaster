#pragma once

#include "Shader.hpp"

namespace Graphics
{
	class Renderer
	{
	private:
		static constexpr int MAX_VERTEX = 2048;
		static constexpr int MAX_INDEX = 512;

		float m_VertexBuffer[MAX_VERTEX];
		unsigned int m_IndexBuffer[MAX_INDEX];

		unsigned int m_VertexCount, m_VertexIndex, m_IndexCount;



	public:
		Renderer();

		void BeginDraw(const Shader& shader);
		void EndDraw();

		void DrawRect(float x, float y, float w, float h, glm::vec4 color);
		void DrawLine(float x1, float y1, float x2, float y2, float w, glm::vec4 color);

	private:
		void EnsureSpace(int vertices, int indices);
		void Flush();

		void AddVertex(float x, float y, glm::vec4 color);
	};
}

