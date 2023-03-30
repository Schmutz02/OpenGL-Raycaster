#pragma once

#include <GL/glew.h>

namespace Graphics
{
	class VertexBuffer
	{
	private:
		unsigned int m_RendererID;

	public:
		VertexBuffer(const void* data, unsigned int size, GLenum usage);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;
	};
}
