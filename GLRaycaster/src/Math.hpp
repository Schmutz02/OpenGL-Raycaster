#pragma once

constexpr float PI = 3.141593f;
constexpr float DEG2RAD = PI / 180;

static float Dist(float x1, float y1, float x2, float y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

static float BoundTo2PI(const float val)
{
	if (val < 0)
		return val + 2 * PI;
	if (val > 2 * PI)
		return val - 2 * PI;
	return val;
}