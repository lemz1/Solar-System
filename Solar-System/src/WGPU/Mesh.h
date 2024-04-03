#pragma once

#include "Util/Math.h"

#include <webgpu/webgpu.hpp>
#include <vector>

class Mesh 
{
public:
    Mesh(
        const std::vector<Vec3>& vertices,
        const std::vector<Vec3>& normals,
        const std::vector<uint32_t>& indices
    );
    ~Mesh();

    void SetVertices(const std::vector<Vec3>& vertices);
    const std::vector<Vec3>& GetVertices() const { return _vertices; }

    void SetNormals(const std::vector<Vec3>& normals);
    const std::vector<Vec3>& GetNormals() const { return _normals; }

    void SetIndices(const std::vector<uint32_t>& indices);
    const std::vector<uint32_t>& GetIndices() const { return _indices; }

    void Draw(wgpu::RenderPassEncoder renderPass) const;
private:
    std::vector<Vec3> _vertices;
    std::vector<Vec3> _normals;
    std::vector<uint32_t> _indices;

    wgpu::Buffer _vertexBuffer;
    wgpu::Buffer _normalBuffer;
    wgpu::Buffer _indexBuffer;
};
