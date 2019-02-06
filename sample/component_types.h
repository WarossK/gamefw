#pragma once

#include "../gamefw/gamefw_random.h"

struct PlayerScript
{
	float x;
	float y;
	float z;

	void move(float dx, float dy, float dz)
	{
		x += dx;
		y += dy;
		z += dz;
	}
};

struct EnemyScript
{
	gamefw::high_speed_random rand;

	float x;
	float y;
	float z;

	void move()
	{
		x += rand(0.001f, 0.01f);
		y += rand(0.001f, 0.01f);
		z += rand(0.001f, 0.01f);
	}
};