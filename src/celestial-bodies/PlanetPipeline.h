#pragma once

#include "util/Math.h"
#include "wgpu/Texture2D.h"
#include <webgpu/webgpu.hpp>

struct PlanetUniform
{
	Mat4 viewProjection;
	Vec2 stScale;
	Vec2 stTiling;
	Vec2 nmScale;
	Vec2 nmTiling;
	float stBlendSharpness;
	float nmBlendSharpness;
	char _padding[8]; // 8 byte padding 
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
	wgpu::Sampler sampler;
	Texture2D* surfaceTexture;
	Texture2D* normalMap;
	wgpu::Buffer uniformBuffer;
	wgpu::BindGroup bindGroup;
	wgpu::PipelineLayout layout;
	wgpu::RenderPipeline pipeline;
};
