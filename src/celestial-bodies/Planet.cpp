#include "Planet.h"

#include "util/AssetManager.h"
#include "celestial-bodies/IcoSphere.h"

using namespace wgpu;

static IcoSphere::MeshData IcoSphereData;

Planet::Planet()
{
	if (IcoSphereData.vertices.size() == 0)
	{
		IcoSphereData = IcoSphere::Generate(5);
	}

	Generate();
}

Planet::~Planet()
{
	delete _mesh;
}

void Planet::Generate()
{
	if (_mesh)
	{
		delete _mesh;
	}
	_mesh = new Mesh(IcoSphereData.vertices, IcoSphereData.normals, IcoSphereData.indices);
}
