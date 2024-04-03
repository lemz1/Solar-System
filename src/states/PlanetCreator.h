#pragma once

#include "core/State.h"
#include "wgpu/DepthTexture.h"
#include "util/Camera.h"
#include "util/CameraController.h"
#include "celestial-bodies/Planet.h"
#include "celestial-bodies/PlanetPipeline.h"

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
	PlanetPipeline* _pipeline = nullptr;
	Planet* _planet = nullptr;
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
