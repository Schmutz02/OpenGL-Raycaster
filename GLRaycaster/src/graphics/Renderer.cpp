#include "Renderer.hpp"

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace Graphics
{
	Renderer::Renderer()
		: m_VertexCount(0), m_VertexIndex(0), m_IndexCount(0)
	{
	}

    void Renderer::BeginDraw(const Shader& shader)
    {
        m_IndexCount = m_VertexCount = 0;

        shader.Bind();
    }

    void Renderer::EndDraw()
    {
        Flush();
    }

    void Renderer::EnsureSpace(int vertices, int indices)
    {
        if (m_VertexIndex + vertices < MAX_VERTEX &&
            m_IndexCount + indices < MAX_INDEX)
            return;

        Flush();
    }

    void Renderer::Flush()
    {
        VertexBuffer vb(m_VertexBuffer, m_VertexIndex * sizeof(float), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(4);

        VertexArray va;
        va.AddBuffer(vb, layout);

        IndexBuffer ib(m_IndexBuffer, m_IndexCount, GL_DYNAMIC_DRAW);

        va.Bind();
        ib.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

        m_IndexCount = m_VertexCount = m_VertexIndex = 0;
    }

    void Renderer::AddVertex(float x, float y, glm::vec4 color)
    {
        m_VertexBuffer[m_VertexIndex++] = x;
        m_VertexBuffer[m_VertexIndex++] = y;
        m_VertexBuffer[m_VertexIndex++] = color.r;
        m_VertexBuffer[m_VertexIndex++] = color.g;
        m_VertexBuffer[m_VertexIndex++] = color.b;
        m_VertexBuffer[m_VertexIndex++] = color.a;
        m_VertexCount++;
    }

    void Renderer::DrawRect(float x, float y, float w, float h, glm::vec4 color)
    {
        EnsureSpace(8, 6);

        m_IndexBuffer[m_IndexCount + 0] = m_VertexCount + 0;
        m_IndexBuffer[m_IndexCount + 1] = m_VertexCount + 1;
        m_IndexBuffer[m_IndexCount + 2] = m_VertexCount + 2;
        m_IndexBuffer[m_IndexCount + 3] = m_VertexCount + 2;
        m_IndexBuffer[m_IndexCount + 4] = m_VertexCount + 1;
        m_IndexBuffer[m_IndexCount + 5] = m_VertexCount + 3;
        m_IndexCount += 6;

        //top left
        AddVertex(x, y, color);
        //bottom left
        AddVertex(x, y + h, color);
        //top right
        AddVertex(x + w, y, color);
        //bottom right
        AddVertex(x + w, y + h, color);
    }

    void Renderer::DrawLine(float x1, float y1, float x2, float y2, float w, glm::vec4 color)
    {
        EnsureSpace(8, 6);

        m_IndexBuffer[m_IndexCount + 0] = m_VertexCount + 0;
        m_IndexBuffer[m_IndexCount + 1] = m_VertexCount + 1;
        m_IndexBuffer[m_IndexCount + 2] = m_VertexCount + 2;
        m_IndexBuffer[m_IndexCount + 3] = m_VertexCount + 2;
        m_IndexBuffer[m_IndexCount + 4] = m_VertexCount + 1;
        m_IndexBuffer[m_IndexCount + 5] = m_VertexCount + 3;
        m_IndexCount += 6;

        float hw = w / 2.0f;
        glm::vec2 a(x1, y1);
        glm::vec2 b(x2, y2);
        glm::vec2 ex = b - a;
        ex = normalize(ex);
        ex *= hw;

        glm::vec2 ey(-ex.y, ex.x);

        glm::vec2 p1 = a - ey;
        glm::vec2 p2 = a + ey;
        glm::vec2 p3 = b - ey;
        glm::vec2 p4 = b + ey;

        //top left
        AddVertex(p1.x, p1.y, color);
        //bottom left
        AddVertex(p2.x, p2.y, color);
        //top right
        AddVertex(p3.x, p3.y, color);
        //bottom right
        AddVertex(p4.x, p4.y, color);
    }
}
