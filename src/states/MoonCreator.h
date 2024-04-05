#pragma once

#include "core/State.h"
#include "wgpu/DepthTexture.h"
#include "util/Camera.h"
#include "util/CameraController.h"
#include "celestial-bodies/Moon.h"
#include "celestial-bodies/MoonPipeline.h"

class MoonCreator : public State
{
public:
	~MoonCreator() override = default;
	
	void OnCreate() override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnDestroy() override;
private:
	DepthTexture* _depthTexture = nullptr;
	MoonPipeline* _pipeline = nullptr;
	Moon* _moon = nullptr;
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
