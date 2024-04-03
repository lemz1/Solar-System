#include "Planet.h"

#include "Celestial-Bodies/IcoSphere.h"
#include "Util/AssetManager.h"

using namespace wgpu;

Planet::Planet()
{
	Generate();
	//_surfaceTexture = AssetManager::LoadTexture2D("Assets/Images/TunisianSpongebob.jpg", TextureFormat::RGBA8Unorm);
	//_normalMap = AssetManager::LoadTexture2D("Assets/Images/TunisianSpongebob.jpg", TextureFormat::RGBA8Unorm);
}

Planet::~Planet()
{
	//delete _normalMap;
	//delete _surfaceTexture;
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
