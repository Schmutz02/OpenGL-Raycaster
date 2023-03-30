#include "Player.hpp"

#include <GLFW/glfw3.h>

Player::Player(float x, float y)
	: m_Pos(x, y), m_Vel(0, 0), m_Angle(0)
{
}

void Player::Update(GLFWwindow& window, float deltaTime)
{
    if (glfwGetKey(&window, GLFW_KEY_A))
    {
        m_Angle -= ROTATION_SPEED * deltaTime;
    	if (m_Angle < 0) { m_Angle += 2 * PI; }
        m_Vel.x = cos(m_Angle) * MOVE_SPEED;
        m_Vel.y = sin(m_Angle) * MOVE_SPEED;
    }
    if (glfwGetKey(&window, GLFW_KEY_D))
    {
        m_Angle += ROTATION_SPEED * deltaTime;
        if (m_Angle > 2 * PI) { m_Angle -= 2 * PI; }
        m_Vel.x = cos(m_Angle) * MOVE_SPEED;
        m_Vel.y = sin(m_Angle) * MOVE_SPEED;
    }
    if (glfwGetKey(&window, GLFW_KEY_W))
    {
        m_Pos += m_Vel * deltaTime;
    }
    if (glfwGetKey(&window, GLFW_KEY_S))
    {
        m_Pos -= m_Vel * deltaTime;
    }
}

void Player::Draw(Graphics::Renderer& renderer)
{
    renderer.DrawRect(m_Pos.x - 5, m_Pos.y - 5, 10, 10, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
}