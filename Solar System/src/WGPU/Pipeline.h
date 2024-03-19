#pragma once

#include <webgpu/webgpu.hpp>
#include <vector>

class RenderPipeline
{
public:
	// shaderModule destruction is handled by the renderpipeline
	RenderPipeline(
		const char* shaderPath,
		const char* vertexEntryPoint,
		const char* fragmentEntryPoint
	);
	~RenderPipeline();

public:
	wgpu::ShaderModule shaderModule;
	wgpu::RenderPipeline pipeline;
};
