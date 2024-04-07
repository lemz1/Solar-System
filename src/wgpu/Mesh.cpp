#include "Mesh.h"

#include "core/Application.h"
#include "debug/Timer.h"

using namespace wgpu;

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
    _vertexBuffer.destroy();
    _vertexBuffer.release();
    _normalBuffer.destroy();
    _normalBuffer.release();
    _indexBuffer.destroy();
    _indexBuffer.release();
}

void Mesh::SetVertices(const Vector<Vec3>& vertices)
{
    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    if (_vertices.size() != vertices.size()) 
    {
        if (_vertexBuffer) 
        {
            _vertexBuffer.destroy();
            _vertexBuffer.release();
        }

        BufferDescriptor bufferDesc;
        bufferDesc.label = "Vertex Buffer (Mesh)";
        bufferDesc.size = vertices.size() * sizeof(Vec3);
        bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
        bufferDesc.mappedAtCreation = false;
        _vertexBuffer = device.createBuffer(bufferDesc);
    }

    _vertices = vertices;

    queue.writeBuffer(_vertexBuffer, 0, _vertices.data(), _vertices.size() * sizeof(Vec3));
}

void Mesh::SetNormals(const Vector<Vec3>& normals)
{
    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    if (_normals.size() != normals.size()) 
    {
        if (_normalBuffer) 
        {
            _normalBuffer.destroy();
            _normalBuffer.release();
        }

        BufferDescriptor bufferDesc;
        bufferDesc.label = "Normal Buffer (Mesh)";
        bufferDesc.size = normals.size() * sizeof(Vec3);
        bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
        bufferDesc.mappedAtCreation = false;
        _normalBuffer = device.createBuffer(bufferDesc);
    }

    _normals = normals;

    queue.writeBuffer(_normalBuffer, 0, _normals.data(), _normals.size() * sizeof(Vec3));
}

void Mesh::SetIndices(const Vector<uint32_t>& indices)
{

    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    if (_indices.size() != indices.size()) 
    {
        if (_indexBuffer) 
        {
            _indexBuffer.destroy();
            _indexBuffer.release();
        }

        BufferDescriptor bufferDesc;
        bufferDesc.label = "Index Buffer (Mesh)";
        bufferDesc.size = indices.size() * sizeof(uint32_t);
        bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Index;
        bufferDesc.mappedAtCreation = false;
        _indexBuffer = device.createBuffer(bufferDesc);
    }

    _indices = indices;

    queue.writeBuffer(_indexBuffer, 0, _indices.data(), _indices.size() * sizeof(uint32_t));
}

void Mesh::Draw(RenderPassEncoder renderPass) const
{
    renderPass.setVertexBuffer(0, _vertexBuffer, 0, _vertices.size() * sizeof(Vec3));
    renderPass.setVertexBuffer(1, _normalBuffer, 0, _normals.size() * sizeof(Vec3));
    renderPass.setIndexBuffer(_indexBuffer, IndexFormat::Uint32, 0, _indices.size() * sizeof(uint32_t));

    renderPass.drawIndexed((uint32_t)_indices.size(), 1, 0, 0, 0);
}

void Mesh::RecalculateNormals()
{
    PROFILE_FUNCTION("Recalculate Normals");
    
    // Calculate face normals
    Vector<Vec3> faceNormals;
    faceNormals.reserve(_indices.size() / 3);
    for (uint32_t i = 0; i < _indices.size(); i += 3) 
    {
        Vec3 edge1 = _vertices[_indices[i + 1]] - _vertices[_indices[i]];
        Vec3 edge2 = _vertices[_indices[i + 2]] - _vertices[_indices[i]];
        Vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));
        faceNormals.emplace_back(faceNormal);
    }

    // Calculate vertex normals
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

    // Average vertex normals out by dividing 
    // by the amount of faces constructed by the vertex
    for (uint32_t i = 0; i < vertexNormals.size(); i++) 
    {
        if (vertexFaceCount[i] == 0) 
        {
            continue;
        }

        vertexNormals[i] /= vertexFaceCount[i];
        vertexNormals[i] = glm::normalize(vertexNormals[i]);
    }

    SetNormals(vertexNormals);
}
