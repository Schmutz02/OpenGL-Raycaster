#pragma once

#include <GL/glew.h>

namespace Graphics
{
	class IndexBuffer
	{
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;

	public:
		IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage);
		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		unsigned int GetCount() const { return m_Count; }
	};
}

