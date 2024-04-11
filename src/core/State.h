#pragma once

#include <webgpu/webgpu.hpp>

#include "core/event/Event.h"

class State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void OnCreate() = 0;
	virtual void OnEvent(const Event& event) = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnDraw(wgpu::RenderPassEncoder renderPass) = 0;
	virtual void OnDrawImGui() = 0;
	virtual void OnDestroy() = 0;
};
