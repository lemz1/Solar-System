#pragma once

#include "util/Math.h"
#include "wgpu/Buffer.h"
#include <webgpu/webgpu.hpp>

struct MoonComputeUniform 
{
    uint32_t numVertices;
};

class MoonComputePipeline
{
public:
	MoonComputePipeline(
		const char* shaderPath,
		uint32_t numVertices
	);
	~MoonComputePipeline();

    Vector<float> Compute(
		wgpu::Buffer heightBuffer,
		wgpu::BindGroup bindGroup
	);
public:
	wgpu::ShaderModule shaderModule = nullptr;
	wgpu::BindGroupLayout bindGroupLayout = nullptr;
	wgpu::PipelineLayout layout = nullptr;
	wgpu::ComputePipeline pipeline = nullptr;
private:
	uint32_t _numVertices = 0;

	Buffer* _mapBuffer = nullptr;
};
