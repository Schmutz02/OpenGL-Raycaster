#include "Raycaster.hpp"

#include "Main.hpp"

Raycaster::Raycaster(Graphics::Renderer& renderer, const int& map)
	: m_Renderer(renderer), m_Map(&map)
{
}

Raycaster::~Raycaster()
{
}

void Raycaster::Draw(const Player& player)
{
	DrawMap2D();
    DrawRays3D(player);
}

void Raycaster::DrawMap2D()
{
    for (int y = 0; y < 8; y++)
 	for (int x = 0; x < 8; x++)
    {
        glm::vec4 color = glm::vec4(0);
        if (m_Map[y * 8 + x] == 1)
            color = glm::vec4(1);

        int sx = x * MAP_S;
        int sy = y * MAP_S;

        m_Renderer.DrawRect(sx+1, sy+1, MAP_S -1, MAP_S -1, color);
    }
}

void Raycaster::DrawRays3D(const Player& player)
{
    float angle = BoundTo2PI(player.Angle() - FOV / 2.0f * DEG2RAD);
    for (int i = 1; i <= FOV * RES; i++)
    {
	    glm::vec2 ray, delta;

	    //---Horizontal Lines---
        glm::vec2 hitH = player.Pos();
        float distH = 1000000;
        int dof = 0;
        float invSlope = -1 / tan(angle);
        if (angle > PI)
        {
	        ray.y = int(player.Pos().y / MAP_S) * MAP_S - 0.0001;
        	ray.x = (player.Pos().y - ray.y) * invSlope + player.Pos().x;
        	delta.y = -MAP_S; delta.x = -delta.y * invSlope;
        }
        if (angle < PI)
        {
	        ray.y = int(player.Pos().y / MAP_S) * MAP_S + MAP_S;
        	ray.x = (player.Pos().y - ray.y) * invSlope + player.Pos().x;
            delta.y = MAP_S; delta.x = -delta.y * invSlope;
        }
        if (angle == 0 || angle == PI)
        {
            ray.x = player.Pos().x; ray.y = player.Pos().y;
        	dof = 8;
        }
        while (dof < 8)
        {
            int x = int(ray.x / MAP_S);
        	int y = int(ray.y / MAP_S);
        	int index = y * 8 + x;
            if (index > 0 && index < 8 * 8 && m_Map[index] == 1)
            {
                hitH = ray;
                distH = distance(ray, player.Pos());
                break;
            }

            ray += delta;
        	dof++;
        }

        //---Vertical Lines---
        glm::vec2 hitV = player.Pos();
        float distV = 1000000;
        dof = 0;
        float negSlope = -tan(angle);
        if (angle > PI / 2 && angle < 3 * PI / 2)
        {
            ray.x = int(player.Pos().x / MAP_S) * MAP_S - 0.0001;
            ray.y = (player.Pos().x - ray.x) * negSlope + player.Pos().y;
        	delta.x = -MAP_S; delta.y = -delta.x * negSlope;
        }
        if (angle < PI / 2 || angle > 3 * PI / 2)
        {
            ray.x = int(player.Pos().x / MAP_S) * MAP_S + MAP_S;
            ray.y = (player.Pos().x - ray.x) * negSlope + player.Pos().y;
        	delta.x = MAP_S; delta.y = -delta.x * negSlope;
        }
        if (angle == PI/2 || angle == 3*PI/2)
        {
            ray.x = player.Pos().x; ray.y = player.Pos().y;
            dof = 8;
        }
        while (dof < 8)
        {
            int x = int(ray.x / MAP_S);
            int y = int(ray.y / MAP_S);
            int index = y * 8 + x;
            if (index > 0 && index < 8 * 8 && m_Map[index] == 1)
            {
                hitV = ray;
                distV = distance(ray, player.Pos());
                break;
            }

            ray += delta;
        	dof++;
        }
        
        float distL = distV;
        glm::vec4 color(0.8f, 0.8f, 0.8f, 1.0f);
        if (distH < distV)
        {
	        ray = hitH;
        	distL = distH;
            color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        }

        m_Renderer.DrawLine(player.Pos().x, player.Pos().y, ray.x, ray.y, 1, glm::vec4(0, 1, 0, 1));

        //---3D Walls---
        float la = BoundTo2PI(player.Angle() - angle);
        distL *= cos(la);
        float lineH = MAP_S * SCREEN_HEIGHT / distL; if (lineH > SCREEN_HEIGHT) { lineH = SCREEN_HEIGHT; }
        float lineO = SCREEN_HEIGHT / 2 - lineH / 2;
        float lineW = SCREEN_WIDTH / 2 / FOV;

        m_Renderer.DrawLine(i * lineW + SCREEN_WIDTH / 2 - lineW / 2, lineO,
                          i * lineW + SCREEN_WIDTH / 2 - lineW / 2, lineO + lineH,
                          lineW, color);
        
        angle = BoundTo2PI(angle + DEG2RAD / RES);
    }
}

