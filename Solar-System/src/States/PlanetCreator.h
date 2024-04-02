#pragma once

#include "Core/State.h"
#include "WGPU/DepthTexture.h"
#include "WGPU/MeshPipeline.h"
#include "Util/Camera.h"
#include "Util/CameraController.h"
#include "Celestial-Bodies/Planet.h"

class PlanetCreator : public State
{
public:
	~PlanetCreator() override = default;
	
	void OnCreate() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnDestroy() override;
private:
	DepthTexture* _depthTexture = nullptr;
	MeshPipeline* _pipeline = nullptr;
	Planet* _planet = nullptr;
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
