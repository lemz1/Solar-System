#include "MoonComputePipeline.h"

#include "core/Application.h"
#include "util/AssetManager.h"
#include "wgpu/DepthTexture.h"
#include "debug/Timer.h"

MoonComputePipeline::MoonComputePipeline(
    const char* shaderPath,
	uint32_t numVertices
)
:	_numVertices(numVertices)
{
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;
	wgpu::TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;

	shaderModule = AssetManager::LoadShaderModule(shaderPath);

	wgpu::ComputePipelineDescriptor pipelineDesc;
	pipelineDesc.label = "Moon Compute Pipeline";
    
    wgpu::ProgrammableStageDescriptor computeStage = wgpu::Default;
    computeStage.entryPoint = "main";
    computeStage.module = shaderModule;
    pipelineDesc.compute = computeStage;

    wgpu::PipelineLayoutDescriptor layoutDesc = wgpu::Default;

	Vector<wgpu::BindGroupLayoutEntry> bindingLayoutEntries(3, wgpu::Default);

	{
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[0];
		bindingLayout.binding = 0;
		bindingLayout.visibility = wgpu::ShaderStage::Compute;
		bindingLayout.buffer.type = wgpu::BufferBindingType::ReadOnlyStorage;
		bindingLayout.buffer.minBindingSize = _numVertices * sizeof(Vec3);
	}

    {
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[1];
		bindingLayout.binding = 1;
		bindingLayout.visibility = wgpu::ShaderStage::Compute;
		bindingLayout.buffer.type = wgpu::BufferBindingType::Storage;
		bindingLayout.buffer.minBindingSize = _numVertices * sizeof(float);
	}

    {
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[2];
		bindingLayout.binding = 2;
		bindingLayout.visibility = wgpu::ShaderStage::Compute;
		bindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
		bindingLayout.buffer.minBindingSize = sizeof(MoonComputeUniform);
	}

	wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.entryCount = (uint32_t)bindingLayoutEntries.size();
	bindGroupLayoutDesc.entries = bindingLayoutEntries.data();
	bindGroupLayout = device.createBindGroupLayout(bindGroupLayoutDesc);
    layoutDesc.bindGroupLayoutCount = 1;
    layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*)&bindGroupLayout;

    layout = device.createPipelineLayout(layoutDesc);
    pipelineDesc.layout = layout;

    pipeline = device.createComputePipeline(pipelineDesc);

	uint32_t size = _numVertices * sizeof(float);
	_mapBuffer = new Buffer(wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead, size);
}

MoonComputePipeline::~MoonComputePipeline()
{
	pipeline.release();
	layout.release();
	bindGroupLayout.release();
	shaderModule.release();
}

Vector<float> MoonComputePipeline::Compute(
	wgpu::Buffer heightBuffer,
	wgpu::BindGroup bindGroup
)
{
	PROFILE_FUNCTION("Compute");

    wgpu::Device device = Application::GetWGPUContext()->device;
    wgpu::Queue queue = Application::GetWGPUContext()->queue;

	wgpu::CommandEncoderDescriptor encoderDesc = wgpu::Default;
	wgpu::CommandEncoder encoder = device.createCommandEncoder(encoderDesc);

	wgpu::ComputePassDescriptor computePassDesc = wgpu::Default;
	computePassDesc.timestampWriteCount = 0;
	computePassDesc.timestampWrites = nullptr;
	wgpu::ComputePassEncoder computePass = encoder.beginComputePass(computePassDesc);

	computePass.setPipeline(pipeline);
	computePass.setBindGroup(0, bindGroup, 0, nullptr);

	float invocationCount = (float)_numVertices;
	constexpr float workGroupSize = 32;
	uint32_t workGroupCount = ceil(invocationCount / workGroupSize);

	computePass.dispatchWorkgroups(workGroupCount, 1, 1);

	computePass.end();
	computePass.release();

	encoder.copyBufferToBuffer(heightBuffer, 0, _mapBuffer->GetBuffer(), 0, _mapBuffer->GetSize());

	wgpu::CommandBuffer commands = encoder.finish(wgpu::CommandBufferDescriptor{});
	queue.submit(commands);
	commands.release();

	encoder.release();

	Vector<float> heights(_numVertices, 0);

	bool done = false;
	auto _handle = _mapBuffer->GetBuffer().mapAsync(wgpu::MapMode::Read, 0, _mapBuffer->GetSize(), [&](wgpu::BufferMapAsyncStatus status) 
	{
		if (status == wgpu::BufferMapAsyncStatus::Success) 
		{
			const float* output = (const float*)_mapBuffer->GetBuffer().getConstMappedRange(0, _mapBuffer->GetSize());
			for (int i = 0; i < _numVertices; i++) 
			{
				heights[i] = output[i];
			}
			_mapBuffer->GetBuffer().unmap();
		}
		done = true;
	});

	while (!done) 
	{
		queue.submit(0, nullptr);
	}

	return heights;
}
