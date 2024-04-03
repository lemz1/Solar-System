#pragma once

#include "util/Math.h"
#include <vector>

namespace IcoSphere
{
	struct MeshData
	{
		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<uint32_t> indices;
	};

	MeshData Generate(uint32_t lod);
};
