#pragma once

#include "Graphics.hpp"
#include "Player.hpp"

class Raycaster
{
private:
	const float FOV = 45;
	const float RES = 1;

	Graphics::Renderer& m_Renderer;
	const int* m_Map;

public:
	Raycaster(Graphics::Renderer& renderer, const int& map);
	~Raycaster();

	void Draw(const Player& player);

private:
	void DrawMap2D();
	void DrawRays3D(const Player& player);
};

