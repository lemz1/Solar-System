#pragma once

#include "util/Math.h"
#include "wgpu/Texture2D.h"
#include <webgpu/webgpu.hpp>

struct MoonUniform
{
	Mat4 viewProjection;
	Vec3 moonColor;
	Vec2 normalMapScale;
	Vec2 normalMapTiling;
	float normalMapBlendSharpness;
	char _padding[16]; // 16 byte padding (makes the errors go away)
};

class MoonPipeline
{
public:
	MoonPipeline(
		const char* shaderPath,
		const char* vertexEntryPoint,
		const char* fragmentEntryPoint
	);
	~MoonPipeline();
public:
	wgpu::ShaderModule shaderModule;
	wgpu::BindGroupLayout bindGroupLayout;
	wgpu::Sampler sampler;
	Texture2D* normalMap;
	wgpu::Buffer uniformBuffer;
	wgpu::BindGroup bindGroup;
	wgpu::PipelineLayout layout;
	wgpu::RenderPipeline pipeline;
};
