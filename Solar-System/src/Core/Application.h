#pragma once

#include "Core/Window.h"
#include "WGPU/WGPUContext.h"
#include "Core/State.h"

class Application
{
public:
	Application(
		uint32_t width,
		uint32_t height,
		const char* title,
		State* initialState
	);
	~Application();

	void Run();

	static void SwitchState(State* nextState);

	static Window* GetWindow() { return _Instance->_window; }
	static WGPUContext* GetWGPUContext() { return _Instance->_wgpuContext; }
	static State* GetState() { return _Instance->_state; }
private:
	static Application* _Instance;

	Window* _window;
	WGPUContext* _wgpuContext;
	State* _state;
};
