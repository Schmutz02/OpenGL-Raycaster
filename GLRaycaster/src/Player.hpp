#pragma once

#include "Graphics.hpp"
#include "Math.hpp"

#include <GLFW/glfw3.h>

class Player
{
public:
	Player(float x, float y);

private:
	glm::vec2 m_Pos, m_Vel;
	float m_Angle;

	const float ROTATION_SPEED = PI;
	const float MOVE_SPEED = 50;
	const float SIZE = 10;



public:
	const glm::vec2& Pos() const { return m_Pos; }
	float Angle() const { return m_Angle; }

	void Update(GLFWwindow& window, float deltaTime);
	void Draw(Graphics::Renderer& renderer);
};

