#pragma once

#include "util/Math.h"
#include <vector>

namespace IcoSphere
{
	struct MeshData
	{
		Vector<Vec3> vertices;
		Vector<Vec3> normals;
		Vector<uint32_t> indices;
	};

	MeshData Generate(uint32_t lod);
};
