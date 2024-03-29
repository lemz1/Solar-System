#pragma once

#include "Core/State.h"
#include "WGPU/MeshPipeline.h"
#include "WGPU/Mesh.h"
#include "Util/Camera.h"
#include "Util/CameraController.h"
#include "Celestial-Bodies/IcoSphere.h"

#include <vector>
#include <webgpu/webgpu.hpp>

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
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
