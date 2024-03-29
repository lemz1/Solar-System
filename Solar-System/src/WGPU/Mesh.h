#pragma once

#include "Util/Math.h"
#include "WGPU/MeshPipeline.h"

#include <webgpu/webgpu.hpp>
#include <vector>

class Mesh 
{
public:
    Mesh(
        const std::vector<Vec3>& vertices,
        const std::vector<Vec2>& texCoords,
        const std::vector<Vec3>& normals,
        const std::vector<uint32_t>& indices,
        const MeshUniform& uniform
    );
    ~Mesh();

    void SetVertices(const std::vector<Vec3>& vertices);
    const std::vector<Vec3>& GetVertices() const { return _vertices; }

    void SetTexCoords(const std::vector<Vec2>& texCoords);
    const std::vector<Vec2>& GetTexCoords() const { return _texCoords; }

    void SetNormals(const std::vector<Vec3>& normals);
    const std::vector<Vec3>& GetNormals() const { return _normals; }

    void SetIndices(const std::vector<uint32_t>& indices);
    const std::vector<uint32_t>& GetIndices() const { return _indices; }

    void Draw(wgpu::RenderPassEncoder renderPass);
public:
    MeshUniform uniform;
private:
    std::vector<Vec3> _vertices;
    std::vector<Vec2> _texCoords;
    std::vector<Vec3> _normals;
    std::vector<uint32_t> _indices;

    wgpu::Buffer _vertexBuffer;
    wgpu::Buffer _texCoordBuffer;
    wgpu::Buffer _normalBuffer;
    wgpu::Buffer _indexBuffer;
};
