#pragma once

#include <vector>
#include <webgpu/webgpu.hpp>
#include "Core/State.h"
#include "WGPU/Pipeline.h"

class PlanetCreator : public State
{
public:
	~PlanetCreator() override = default;
	
	void OnCreate() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnDestroy() override;
private:
	std::vector<float> pointData;
	std::vector<uint16_t> indexData;
	wgpu::Buffer vertexBuffer = nullptr;
	wgpu::Buffer indexBuffer = nullptr;
	RenderPipeline* pipeline = nullptr;
};
