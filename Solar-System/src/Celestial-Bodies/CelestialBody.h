#pragma once

#include "WGPU/Mesh.h"

class CelestialBody
{
public:
	CelestialBody();
	~CelestialBody();

	const Mesh* GetMesh() const { return _mesh; }
private:
	Mesh* _mesh;
};
