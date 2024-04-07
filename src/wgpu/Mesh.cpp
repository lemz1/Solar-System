#include "Mesh.h"

#include "core/Application.h"

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
