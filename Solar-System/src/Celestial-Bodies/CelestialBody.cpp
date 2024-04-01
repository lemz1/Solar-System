#include "CelestialBody.h"

#include "Celestial-Bodies/IcoSphere.h"

CelestialBody::CelestialBody()
{
	_mesh = IcoSphere::Generate(5);
}

CelestialBody::~CelestialBody()
{
	delete _mesh;
}
