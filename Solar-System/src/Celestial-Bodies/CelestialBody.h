#pragma once

#include "WGPU/Mesh.h"

class CelestialBody
{
public:
	CelestialBody();
	~CelestialBody();
private:
	Mesh* _mesh;
};
