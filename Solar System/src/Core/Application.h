#pragma once

#include "Core/Window.h"
#include "Core/WGPUContext.h"
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

	static Window* GetWindow() { return _instance->_window; }
	static WGPUContext* GetWGPUContext() { return _instance->_wgpuContext; }
	static State* GetState() { return _instance->_state; }
private:
	static Application* _instance;

	Window* _window;
	WGPUContext* _wgpuContext;
	State* _state;
};
