#pragma once

#include "wgpu/Texture2D.h"
#include "wgpu/Buffer.h"
#include "wgpu/Mesh.h"
#include "util/FractalNoiseSettings.h"
#include "celestial/MoonRenderPipeline.h"
#include "celestial/MoonComputePipeline.h"

class Moon
{
public:
	Moon();
	~Moon();

	void Generate(uint32_t subdivisions);

	void Draw(wgpu::RenderPassEncoder renderPass) const;

	void SetNoiseSettings(const FractalNoiseSettings& noiseSettings) { _noiseSettings = noiseSettings; }
	const FractalNoiseSettings& GetNoiseSettings() const { return _noiseSettings; }

	const Mesh* GetMesh() const { return _mesh; }

	const Buffer* GetComputeUniformBuffer() const { return _computeUniformBuffer; }

	const MoonComputePipeline* GetComputePipeline() const { return _computePipeline; }
	const MoonRenderPipeline* GetRenderPipeline() const { return _renderPipeline; }
private:
	void CreateComputeBuffers();
private:
	FractalNoiseSettings _noiseSettings = {};
	Mesh* _mesh = nullptr;

	wgpu::BindGroup _computeBindGroup = nullptr;
	Buffer* _computeVertexBuffer = nullptr;
	Buffer* _computeHeightBuffer = nullptr;
	Buffer* _computeUniformBuffer = nullptr;

	MoonComputePipeline* _computePipeline = nullptr;
    MoonRenderPipeline* _renderPipeline = nullptr;
};
