#include "Moon.h"

#include "util/AssetManager.h"
#include "celestial/IcoSphere.h"
#include "core/Application.h"
#include "debug/Timer.h"

static IcoSphere::MeshData IcoSphereData;

Moon::Moon()
{
	_renderPipeline = new MoonRenderPipeline("assets/shaders/MoonRenderShader.wgsl");

	Generate(4);
}

Moon::~Moon()
{
	_computeBindGroup.release();
	delete _computeUniformBuffer;
	delete _computeHeightBuffer;
	delete _computeVertexBuffer;
	delete _mesh;
	delete _computePipeline;
	delete _renderPipeline;
}

void Moon::Generate(uint32_t subdivisions)
{
	// vertexAmount = 10 * T + 2
	if (IcoSphereData.vertices.size() != 10 * pow(4, subdivisions) + 2)
	{
		IcoSphereData.vertices.clear();
		IcoSphereData.indices.clear();
		IcoSphereData = IcoSphere::Generate(subdivisions);

		if (_computePipeline) 
		{
			delete _computePipeline;
			delete _computeVertexBuffer;
			delete _computeHeightBuffer;
			delete _computeUniformBuffer;
		}
		_computePipeline = new MoonComputePipeline("assets/shaders/MoonComputeShader.wgsl", IcoSphereData.vertices.size());
		CreateComputeBuffers();
	}

	Vector<float> heights = _computePipeline->Compute(_computeHeightBuffer->GetBuffer(), _computeBindGroup);
	
	Vector<Vec3> vertices = IcoSphereData.vertices;

	for (int i = 0; i < vertices.size(); i++) 
	{
		vertices[i] = vertices[i] * heights[i];
	}

	if (!_mesh)
	{
		// im using vertices in normals as a placeholder
		_mesh = new Mesh(vertices, vertices, IcoSphereData.indices);
		_mesh->RecalculateNormals();
	} 
	else 
	{
		_mesh->SetVertices(vertices);
		_mesh->SetIndices(IcoSphereData.indices);
		_mesh->RecalculateNormals();
	}
}

void Moon::Draw(wgpu::RenderPassEncoder renderPass) const
{
	_renderPipeline->BindToRenderPass(renderPass);

	_mesh->Draw(renderPass);
}

void Moon::CreateComputeBuffers()
{
	wgpu::Device device = Application::GetWGPUContext()->device;

	{ // Compute Vertex Buffer
		uint32_t size = (uint32_t)(IcoSphereData.vertices.size() * sizeof(Vec3));
		_computeVertexBuffer = new Buffer(wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopyDst, size);
		_computeVertexBuffer->Write(IcoSphereData.vertices.data(), size);
	}

	{ // Compute Height Buffer
		uint32_t size = (uint32_t)(IcoSphereData.vertices.size() * sizeof(float));
		_computeHeightBuffer = new Buffer(wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopySrc, size);
	}

	{ // Compute Uniform Buffer
		uint32_t size = (uint32_t)sizeof(MoonComputeUniform);
		_computeUniformBuffer = new Buffer(wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst, size);

		MoonComputeUniform uniformData = {};
		uniformData.numVertices = (uint32_t)IcoSphereData.vertices.size();

		_computeUniformBuffer->Write(&uniformData, size);
	}

	wgpu::BindGroupDescriptor bindGroupDesc = wgpu::Default;
	bindGroupDesc.layout = _computePipeline->bindGroupLayout;

	Vector<wgpu::BindGroupEntry> entries(3, wgpu::Default);

	{
		wgpu::BindGroupEntry& entry = entries[0];
		entry.binding = 0;
		entry.offset = 0;
		entry.buffer = _computeVertexBuffer->GetBuffer();
		entry.size = _computeVertexBuffer->GetSize();
	}

	{
		wgpu::BindGroupEntry& entry = entries[1];
		entry.binding = 1;
		entry.offset = 0;
		entry.buffer = _computeHeightBuffer->GetBuffer();
		entry.size = _computeHeightBuffer->GetSize();
	}

	{
		wgpu::BindGroupEntry& entry = entries[2];
		entry.binding = 2;
		entry.offset = 0;
		entry.buffer = _computeUniformBuffer->GetBuffer();
		entry.size = _computeUniformBuffer->GetSize();
	}

	bindGroupDesc.entryCount = (uint32_t)entries.size();
	bindGroupDesc.entries = entries.data();

	_computeBindGroup = device.createBindGroup(bindGroupDesc);
}
