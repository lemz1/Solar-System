#pragma once

#include "core/State.h"
#include "core/event/WindowEvent.h"
#include "util/Camera.h"
#include "util/CameraController.h"
#include "celestial/Moon.h"

class MoonCreator : public State
{
public:
	~MoonCreator() override = default;
	
	void OnCreate() override;
	void OnEvent(const Event& event) override;
	void OnUpdate(float deltaTime) override;
	void OnDraw(wgpu::RenderPassEncoder renderPass) override;
	void OnDrawImGui() override;
	void OnDestroy() override;
private:
	void OnResize(const WindowResizeEvent& event);
private:
	Moon* _moon = nullptr;
	Camera* _camera = nullptr;
	CameraController* _cameraController = nullptr;
};
