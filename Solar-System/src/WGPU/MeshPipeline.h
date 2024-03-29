#pragma once

#include <webgpu/webgpu.hpp>
#include <vector>

struct MeshUniform
{
	float uTime;
};

class MeshPipeline
{
public:
	// shaderModule destruction is handled by this instance
	MeshPipeline(
		const char* shaderPath,
		const char* vertexEntryPoint,
		const char* fragmentEntryPoint
	);
	~MeshPipeline();

	operator wgpu::RenderPipeline() { return pipeline; }
	operator wgpu::ShaderModule() { return shaderModule; }
public:
	wgpu::ShaderModule shaderModule;
	wgpu::BindGroupLayout bindGroupLayout;
	wgpu::Buffer uniformBuffer;
	wgpu::BindGroup bindGroup;
	wgpu::PipelineLayout layout;
	wgpu::RenderPipeline pipeline;
};
