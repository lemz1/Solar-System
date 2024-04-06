#pragma once

#include "core/State.h"
#include "core/event/WindowEvent.h"
#include "wgpu/DepthTexture.h"
#include "util/Camera.h"
#include "util/CameraController.h"
#include "celestial/Moon.h"
#include "celestial/MoonPipeline.h"

class MoonCreator : public State
{
public:
	~MoonCreator() override = default;
	
	void OnCreate() override;
	void OnEvent(const Event& event) override;
	void OnUpdate(float deltaTime) override;
	void OnDraw() override;
	void OnDestroy() override;
private:
	void OnResize(const WindowResizeEvent& event);
private:
	DepthTexture* _depthTexture = nullptr;
	MoonPipeline* _pipeline = nullptr;
	Moon* _moon = nullptr;
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
