#pragma once

#include "core/Window.h"
#include "core/State.h"
#include "wgpu/WGPUContext.h"
#include "mygui/MyGuiContext.h"

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

	static float GetTime() { return _Instance->_time; }
	static Window* GetWindow() { return _Instance->_window; }
	static WGPUContext* GetWGPUContext() { return _Instance->_wgpuContext; }
	static State* GetState() { return _Instance->_state; }
private:
	static Application* _Instance;

	float _time = 0;
	Window* _window = nullptr;
	WGPUContext* _wgpuContext = nullptr;
	MyGuiContext* _myguiContext = nullptr;
	State* _state = nullptr;
};
