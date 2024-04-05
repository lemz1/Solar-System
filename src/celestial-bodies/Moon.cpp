#include "Moon.h"

#include "util/AssetManager.h"
#include "celestial-bodies/IcoSphere.h"

using namespace wgpu;

static IcoSphere::MeshData IcoSphereData;

Moon::Moon()
{
	if (IcoSphereData.vertices.size() == 0)
	{
		IcoSphereData = IcoSphere::Generate(5);
	}

	Generate();
}

Moon::~Moon()
{
	delete _mesh;
}

void Moon::Generate()
{
	if (_mesh)
	{
		delete _mesh;
	}
	_mesh = new Mesh(IcoSphereData.vertices, IcoSphereData.normals, IcoSphereData.indices);
}
