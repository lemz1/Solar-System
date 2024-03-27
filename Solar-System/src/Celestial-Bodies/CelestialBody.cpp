#include "CelestialBody.h"

#include "Celestial-Bodies/IcoSphere.h"

CelestialBody::CelestialBody()
{
	_mesh = IcoSphere::Generate(0);
}

CelestialBody::~CelestialBody()
{
	delete _mesh;
}
