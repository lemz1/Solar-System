#pragma once

#include "Util/Math.h"

struct FractalNoiseSettings
{
	uint32_t seed = 0;
	Vec2 offset{ 0.0f };
	uint8_t octaves = 8;
	float persistence = 0.5f;
	float lacunarity = 1.0f;
	float scale = 1.0f;
};
