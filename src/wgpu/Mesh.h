#pragma once

#include "util/Math.h"

#include <webgpu/webgpu.hpp>
#include <vector>

class Mesh 
{
public:
    Mesh(
        const Vector<Vec3>& vertices,
        const Vector<Vec3>& normals,
        const Vector<uint32_t>& indices
    );
    ~Mesh();

    void Draw(wgpu::RenderPassEncoder renderPass) const;

    void RecalculateNormals();

    void SetVertices(const Vector<Vec3>& vertices);
    const Vector<Vec3>& GetVertices() const { return _vertices; }

    void SetNormals(const Vector<Vec3>& normals);
    const Vector<Vec3>& GetNormals() const { return _normals; }

    void SetIndices(const Vector<uint32_t>& indices);
    const Vector<uint32_t>& GetIndices() const { return _indices; }

    wgpu::Buffer GetVertexBuffer() const { return _vertexBuffer; }
    wgpu::Buffer GetNormalBuffer() const { return _normalBuffer; }
    wgpu::Buffer GetIndexBuffer() const { return _indexBuffer; }
private:
    Vector<Vec3> _vertices;
    Vector<Vec3> _normals;
    Vector<uint32_t> _indices;

    wgpu::Buffer _vertexBuffer = nullptr;
    wgpu::Buffer _normalBuffer = nullptr;
    wgpu::Buffer _indexBuffer = nullptr;
};
