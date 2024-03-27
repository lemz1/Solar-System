#pragma once

#include <webgpu/webgpu.hpp>
#include <vector>

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
	wgpu::RenderPipeline pipeline;
};
