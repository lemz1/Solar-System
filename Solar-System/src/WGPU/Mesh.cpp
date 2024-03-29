#include "Mesh.h"

#include "Core/Application.h"

using namespace wgpu;

Mesh::Mesh(
    const std::vector<Vec3>& vertices,
    const std::vector<Vec2>& texCoords,
    const std::vector<Vec3>& normals,
    const std::vector<uint32_t>& indices,
    const MeshUniform& uniform
)
:   _vertexBuffer(nullptr),
    _texCoordBuffer(nullptr),
    _normalBuffer(nullptr),
    _indexBuffer(nullptr),
    uniform(uniform)
{
    SetVertices(vertices);
    SetTexCoords(texCoords);
    SetNormals(normals);
    SetIndices(indices);
}

Mesh::~Mesh() 
{
    _vertexBuffer.destroy();
    _vertexBuffer.release();
    _texCoordBuffer.destroy();
    _texCoordBuffer.release();
    _normalBuffer.destroy();
    _normalBuffer.release();
    _indexBuffer.destroy();
    _indexBuffer.release();
}

void Mesh::SetVertices(const std::vector<Vec3>& vertices)
{
    _vertices = vertices;

    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    BufferDescriptor bufferDesc;
    bufferDesc.label = "Vertex Buffer (Mesh)";
    bufferDesc.size = _vertices.size() * sizeof(Vec3);
    bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
    bufferDesc.mappedAtCreation = false;
    _vertexBuffer = device.createBuffer(bufferDesc);
    queue.writeBuffer(_vertexBuffer, 0, _vertices.data(), bufferDesc.size);
}

void Mesh::SetTexCoords(const std::vector<Vec2>& texCoords)
{
    _texCoords = texCoords;

    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    BufferDescriptor bufferDesc;
    bufferDesc.label = "TexCoord Buffer (Mesh)";
    bufferDesc.size = _texCoords.size() * sizeof(Vec2);
    bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
    bufferDesc.mappedAtCreation = false;
    _texCoordBuffer = device.createBuffer(bufferDesc);
    queue.writeBuffer(_texCoordBuffer, 0, _texCoords.data(), bufferDesc.size);
}

void Mesh::SetNormals(const std::vector<Vec3>& normals)
{
    _normals = normals;

    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    BufferDescriptor bufferDesc;
    bufferDesc.label = "Normal Buffer (Mesh)";
    bufferDesc.size = _normals.size() * sizeof(Vec3);
    bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Vertex;
    bufferDesc.mappedAtCreation = false;
    _normalBuffer = device.createBuffer(bufferDesc);
    queue.writeBuffer(_normalBuffer, 0, _normals.data(), bufferDesc.size);
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices)
{
    _indices = indices;

    Device device = Application::GetWGPUContext()->device;
    Queue queue = Application::GetWGPUContext()->queue;

    BufferDescriptor bufferDesc;
    bufferDesc.label = "Index Buffer (Mesh)";
    bufferDesc.size = _indices.size() * sizeof(uint32_t);
    bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Index;
    bufferDesc.mappedAtCreation = false;
    _indexBuffer = device.createBuffer(bufferDesc);
    queue.writeBuffer(_indexBuffer, 0, _indices.data(), bufferDesc.size);
}

void Mesh::Draw(RenderPassEncoder renderPass) 
{
    renderPass.setVertexBuffer(0, _vertexBuffer, 0, _vertices.size() * sizeof(Vec3));
    renderPass.setVertexBuffer(1, _texCoordBuffer, 0, _texCoords.size() * sizeof(Vec2));
    renderPass.setVertexBuffer(2, _normalBuffer, 0, _normals.size() * sizeof(Vec3));
    renderPass.setIndexBuffer(_indexBuffer, IndexFormat::Uint32, 0, _indices.size() * sizeof(uint32_t));

    renderPass.drawIndexed(_indices.size(), 1, 0, 0, 0);
}
