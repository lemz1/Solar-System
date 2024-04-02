#include "Planet.h"

#include "Celestial-Bodies/IcoSphere.h"

using namespace wgpu;

Planet::Planet()
{
	Generate();
	//_surfaceTexture = new Texture2D(nullptr, 256, 256, TextureFormat::RGBA8Unorm);
	//_normalMap = new Texture2D(nullptr, 256, 256, TextureFormat::RGBA8Unorm);
}

Planet::~Planet()
{
	delete _normalMap;
	delete _surfaceTexture;
	delete _mesh;
}

void Planet::Generate()
{
	if (_mesh)
	{
		delete _mesh;
	}
	_mesh = IcoSphere::Generate(5);
}
