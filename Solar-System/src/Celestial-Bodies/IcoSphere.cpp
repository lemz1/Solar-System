#include "IcoSphere.h"

#include <vector>
#include "Util/Math.h"

//TODO: finish this

// https://en.wikipedia.org/wiki/Regular_icosahedron 
// Construction: using given coordinates

static constexpr float GoldenRatio = 1.618033988749f;

static constexpr Vec3 IcosahedronVertices[] = {
	Vec3(-1,  GoldenRatio, 0),
	Vec3( 1,  GoldenRatio, 0),
	Vec3(-1, -GoldenRatio, 0),
	Vec3( 1, -GoldenRatio, 0),

	Vec3(0, -1,  GoldenRatio),
	Vec3(0,  1,  GoldenRatio),
	Vec3(0, -1, -GoldenRatio),
	Vec3(0,  1, -GoldenRatio),

	Vec3( GoldenRatio, 0, -1),
	Vec3( GoldenRatio, 0,  1),
	Vec3(-GoldenRatio, 0, -1),
	Vec3(-GoldenRatio, 0,  1),
};

static constexpr uint32_t IcosahedronIndices[] = {
	0, 1, 5,
	0, 1, 7,
	0, 5, 11,
	0, 7, 10,
	0, 10, 11,
	
};

namespace IcoSphere
{
	Mesh* Generate(uint32_t lod)
	{
		std::vector<Vec3> vertices;
		std::vector<uint32_t> indices;

		// subdivide the faces of the icosahedron
		// and project the vertices on the sphere
		for (uint32_t i = 0; i < lod; i++)
		{
			std::vector<Vec3> newVertices;
			std::vector<uint32_t> newIndices;

			for (uint32_t triangleIndex = 0; triangleIndex < indices.size(); triangleIndex += 3)
			{
				Vec3 vertexA = vertices[indices[triangleIndex]];
				Vec3 vertexB = vertices[indices[triangleIndex]];
				Vec3 vertexC = vertices[indices[triangleIndex]];

				Vec3 vertexAB = (vertexA + vertexB) / 2.f;
				Vec3 vertexAC = (vertexA + vertexC) / 2.f;
				Vec3 vertexBC = (vertexB + vertexC) / 2.f;

				newVertices.emplace_back();
				newIndices.emplace_back();
			}

			vertices = newVertices;
			indices = newIndices;
		}

		std::vector<Vec2> texCoords;
		std::vector<Vec3> normals;

		return new Mesh(vertices, texCoords, normals, indices);
	}
}
