#include "IcoSphere.h"

#include <unordered_map>
#include "debug/Timer.h"

// https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
// using this for icosahedron tables
static constexpr float X = 0.525731112119133606f;
static constexpr float Z = 0.850650808352039932f;
static constexpr float N = 0.f;

static const std::vector<Vec3> icosahedronVertices = {
	{-X,N,Z}, {X,N,Z  }, {-X,N,-Z}, {X,N,-Z },
	{N,Z,X }, {N,Z,-X }, {N,-Z,X }, {N,-Z,-X},
	{Z,X,N }, {-Z,X, N}, {Z,-X,N }, {-Z,-X,N}
};

static const std::vector<uint32_t> icosahedronIndices = {
	0,4,1,
	0,9,4,
	9,5,4, 
	4,5,8,
	4,8,1,
	8,10,1,
	8,3,10,
	5,3,8,
	5,2,3,
	2,7,3,
	7,10,3,
	7,6,10,
	7,11,6,
	11,0,6,
	0,1,6,
	6,1,10,
	9,0,11,
	9,11,2,
	9,2,5,
	7,2,11,
};

struct Vec3Hash
{
	size_t operator()(const Vec3& v) const
	{
		size_t h1 = std::hash<float>()(v.x);
		size_t h2 = std::hash<float>()(v.y);
		size_t h3 = std::hash<float>()(v.z);
		return h1 ^ h2 ^ h3;
	}
};

struct Vec3Equal
{
	bool operator()(const Vec3& lhs, const Vec3& rhs) const
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
};

namespace IcoSphere
{
	MeshData Generate(uint32_t subdivisions)
	{
		PROFILE_FUNCTION("IcoSphere Generation");

		std::vector<Vec3> icoSphereVertices = icosahedronVertices;
		std::vector<uint32_t> icoSphereIndices = icosahedronIndices;

		// subdivide the faces of the icosahedron
		// and project the vertices onto the sphere
		for (uint32_t i = 0; i < subdivisions; i++)
		{
			// https://en.wikipedia.org/wiki/Geodesic_polyhedron for T
			uint32_t T = pow(4, i + 1);
			std::vector<Vec3> newVertices;
			newVertices.reserve(10 * T + 2); // 10 * T + 2 (Vertices)
			std::vector<uint32_t> newIndices;
			newIndices.reserve(20 * T * 3); // 20 * T (Faces)
			std::unordered_map<Vec3, uint32_t, Vec3Hash, Vec3Equal> vertexMap;

			for (uint32_t triangleIndex = 0; triangleIndex < icoSphereIndices.size(); triangleIndex += 3)
			{
				Vec3& vertexA = icoSphereVertices[icoSphereIndices[triangleIndex]];
				Vec3& vertexB = icoSphereVertices[icoSphereIndices[triangleIndex + 1]];
				Vec3& vertexC = icoSphereVertices[icoSphereIndices[triangleIndex + 2]];

				Vec3 vertexAB = (vertexA + vertexB) / 2.0f;
				Vec3 vertexBC = (vertexB + vertexC) / 2.0f;
				Vec3 vertexCA = (vertexC + vertexA) / 2.0f;
				
				if (!vertexMap.count(vertexA))
				{
					newVertices.emplace_back(glm::normalize(vertexA));
					vertexMap.insert(std::make_pair(vertexA, newVertices.size() - 1));
				}
				if (!vertexMap.count(vertexB))
				{
					newVertices.emplace_back(glm::normalize(vertexB));
					vertexMap.insert(std::make_pair(vertexB, newVertices.size() - 1));
				}
				if (!vertexMap.count(vertexC))
				{
					newVertices.emplace_back(glm::normalize(vertexC));
					vertexMap.insert(std::make_pair(vertexC, newVertices.size() - 1));
				}
				if (!vertexMap.count(vertexAB))
				{
					newVertices.emplace_back(glm::normalize(vertexAB));
					vertexMap.insert(std::make_pair(vertexAB, newVertices.size() - 1));
				}
				if (!vertexMap.count(vertexBC))
				{
					newVertices.emplace_back(glm::normalize(vertexBC));
					vertexMap.insert(std::make_pair(vertexBC, newVertices.size() - 1));
				}
				if (!vertexMap.count(vertexCA))
				{
					newVertices.emplace_back(glm::normalize(vertexCA));
					vertexMap.insert(std::make_pair(vertexCA, newVertices.size() - 1));
				}

				uint32_t vertexAIndex = vertexMap.at(vertexA);
				uint32_t vertexBIndex = vertexMap.at(vertexB);
				uint32_t vertexCIndex = vertexMap.at(vertexC);
				uint32_t vertexABIndex = vertexMap.at(vertexAB);
				uint32_t vertexBCIndex = vertexMap.at(vertexBC);
				uint32_t vertexCAIndex = vertexMap.at(vertexCA);

				newIndices.emplace_back(vertexAIndex);
				newIndices.emplace_back(vertexABIndex);
				newIndices.emplace_back(vertexCAIndex);

				newIndices.emplace_back(vertexBIndex);
				newIndices.emplace_back(vertexBCIndex);
				newIndices.emplace_back(vertexABIndex);

				newIndices.emplace_back(vertexCIndex);
				newIndices.emplace_back(vertexCAIndex);
				newIndices.emplace_back(vertexBCIndex);

				newIndices.emplace_back(vertexABIndex);
				newIndices.emplace_back(vertexBCIndex);
				newIndices.emplace_back(vertexCAIndex);
			}

			icoSphereVertices = newVertices;
			icoSphereIndices = newIndices;
		}

		// normals are the same as vertices
		return MeshData{ icoSphereVertices, icoSphereVertices, icoSphereIndices };
	}
}
