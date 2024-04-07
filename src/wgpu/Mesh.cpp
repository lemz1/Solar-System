#include "Mesh.h"

#include "core/Application.h"
#include "debug/Timer.h"

Mesh::Mesh(
    const Vector<Vec3>& vertices,
    const Vector<Vec3>& normals,
    const Vector<uint32_t>& indices
)
{
    SetVertices(vertices);
    SetNormals(normals);
    SetIndices(indices);
}

Mesh::~Mesh() 
{
    delete _indexBuffer;
    delete _normalBuffer;
    delete _vertexBuffer;
}

void Mesh::SetVertices(const Vector<Vec3>& vertices)
{
    if (_vertices.size() != vertices.size()) 
    {
        if (_vertexBuffer) 
        {
            delete _vertexBuffer;
        }

        uint32_t size = vertices.size() * sizeof(Vec3);
        _vertexBuffer = new Buffer(wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst, size);
    }

    _vertices = vertices;

    _vertexBuffer->Write(_vertices.data(), _vertices.size() * sizeof(Vec3));
}

void Mesh::SetNormals(const Vector<Vec3>& normals)
{
    if (_normals.size() != normals.size()) 
    {
        if (_normalBuffer) 
        {
            delete _normalBuffer;
        }

        uint32_t size = normals.size() * sizeof(Vec3);
        _normalBuffer = new Buffer(wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst, size);
    }

    _normals = normals;

    _normalBuffer->Write(_normals.data(), _normals.size() * sizeof(Vec3));
}

void Mesh::SetIndices(const Vector<uint32_t>& indices)
{
    if (_indices.size() != indices.size()) 
    {
        if (_indexBuffer) 
        {
            delete _indexBuffer;
        }

        uint32_t size = indices.size() * sizeof(uint32_t);
        _indexBuffer = new Buffer(wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst, size);
    }

    _indices = indices;

    _indexBuffer->Write(_indices.data(), _indices.size() * sizeof(uint32_t));
}

void Mesh::Draw(wgpu::RenderPassEncoder renderPass) const
{
    renderPass.setVertexBuffer(0, _vertexBuffer->GetBuffer(), 0, _vertices.size() * sizeof(Vec3));
    renderPass.setVertexBuffer(1, _normalBuffer->GetBuffer(), 0, _normals.size() * sizeof(Vec3));
    renderPass.setIndexBuffer(_indexBuffer->GetBuffer(), wgpu::IndexFormat::Uint32, 0, _indices.size() * sizeof(uint32_t));

    renderPass.drawIndexed((uint32_t)_indices.size(), 1, 0, 0, 0);
}

// this function is quite expensive
// especially on Debug, but Release is fine
void Mesh::RecalculateNormals()
{
    PROFILE_FUNCTION("Recalculate Normals");
    
    // Calculate face normals (most expensive)
    Vector<Vec3> faceNormals;
    faceNormals.reserve(_indices.size() / 3);
    for (uint32_t i = 0; i < _indices.size(); i += 3) 
    {
        Vec3 edge1 = _vertices[_indices[i + 1]] - _vertices[_indices[i]];
        Vec3 edge2 = _vertices[_indices[i + 2]] - _vertices[_indices[i]];
        Vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));
        faceNormals.emplace_back(faceNormal);
    }

    // Calculate vertex normals (mid expensive)
    Vector<Vec3> vertexNormals(_vertices.size(), Vec3(0.0f));
    Vector<uint32_t> vertexFaceCount(_vertices.size(), 0);
    for (uint32_t i = 0; i < _indices.size(); i += 3) 
    {
        for (uint32_t j = 0; j < 3; j++) 
        {
            uint32_t vertexIndex = _indices[i + j];
            vertexNormals[vertexIndex] += faceNormals[i / 3];
            vertexFaceCount[vertexIndex] += 1;
        }
    }

    // Specific to icosphere:
    // i could skip this step, because all vertices have face count 6
    // except the 12 original vertices, they have face count 5
    // dividing all vertexNormals by 6 is a good enough approximation 
    // maybe i will do that later in a seperate function

    // Average vertex normals out by dividing 
    // by the amount of faces constructed by the vertex
    // (least expensive)
    for (uint32_t i = 0; i < vertexNormals.size(); i++) 
    {
        if (vertexFaceCount[i] == 0) 
        {
            continue;
        }

        vertexNormals[i] /= vertexFaceCount[i];
        // pretty sure this is not necessary
        //vertexNormals[i] = glm::normalize(vertexNormals[i]);
    }
    SetNormals(vertexNormals);
}
