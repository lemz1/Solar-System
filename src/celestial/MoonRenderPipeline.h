#pragma once

#include "util/Math.h"
#include "wgpu/Texture2D.h"
#include <webgpu/webgpu.hpp>

struct MoonRenderUniform
{
	Mat4 viewProjection;
	Vec3 moonColor;
	Vec2 normalMapScale;
	Vec2 normalMapTiling;
	float normalMapBlendSharpness;
	char _padding[16]; // 16 byte padding (makes the errors go away)
};

class MoonRenderPipeline
{
public:
	MoonRenderPipeline(const char* shaderPath);
	~MoonRenderPipeline();

	void BindToRenderPass(wgpu::RenderPassEncoder renderPass);
public:
	wgpu::ShaderModule shaderModule = nullptr;
	wgpu::BindGroupLayout bindGroupLayout = nullptr;
	wgpu::Sampler sampler = nullptr;
	Texture2D* normalMap = nullptr;
	wgpu::Buffer uniformBuffer = nullptr;
	wgpu::BindGroup bindGroup = nullptr;
	wgpu::PipelineLayout layout = nullptr;
	wgpu::RenderPipeline pipeline = nullptr;
};
