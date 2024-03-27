#pragma once

#include <vector>
#include <webgpu/webgpu.hpp>
#include "Core/State.h"
#include "WGPU/MeshPipeline.h"
#include "WGPU/Mesh.h"

class PlanetCreator : public State
{
public:
	~PlanetCreator() override = default;
	
	void OnCreate() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnDestroy() override;
private:
	MeshPipeline* _pipeline = nullptr;
	Mesh* _mesh = nullptr;
};
