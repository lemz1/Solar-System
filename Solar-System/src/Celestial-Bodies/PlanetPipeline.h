#pragma once

#include "Util/Math.h"
#include "WGPU/Texture2D.h"
#include <webgpu/webgpu.hpp>

struct PlanetUniform
{
	Mat4 modelViewProjection;
};

class PlanetPipeline
{
public:
	// shaderModule destruction is handled by this instance
	PlanetPipeline(
		const char* shaderPath,
		const char* vertexEntryPoint,
		const char* fragmentEntryPoint
	);
	~PlanetPipeline();
public:
	wgpu::ShaderModule shaderModule;
	wgpu::BindGroupLayout bindGroupLayout;
	Texture2D* surfaceTexture;
	Texture2D* normalMap;
	wgpu::Buffer uniformBuffer;
	wgpu::BindGroup bindGroup;
	wgpu::PipelineLayout layout;
	wgpu::RenderPipeline pipeline;
};
