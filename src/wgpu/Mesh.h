#pragma once

#include "util/Math.h"
#include "wgpu/Buffer.h"

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

    const Buffer* GetVertexBuffer() const { return _vertexBuffer; }
    const Buffer* GetNormalBuffer() const { return _normalBuffer; }
    const Buffer* GetIndexBuffer() const { return _indexBuffer; }
private:
    Vector<Vec3> _vertices;
    Vector<Vec3> _normals;
    Vector<uint32_t> _indices;

    Buffer* _vertexBuffer = nullptr;
    Buffer* _normalBuffer = nullptr;
    Buffer* _indexBuffer = nullptr;
};
