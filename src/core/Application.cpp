#include "Application.h"

#include "core/Input.h"

Application* Application::_Instance = nullptr;

Application::Application(
	uint32_t width, 
	uint32_t height, 
	const char* title,
	State* initialState
)
{
	if (_Instance)
	{
		return;
	}


	_Instance = this;

	_window = new Window(width, height, title);

	_wgpuContext = new WGPUContext(_window->_handle, _window->_width, _window->_height);

	_state = initialState;
	_state->OnCreate();
}

Application::~Application()
{
	_state->OnDestroy();
	delete _state;

	delete _wgpuContext;
	delete _window;
}

void Application::Run()
{
	_time = 0;

	while (!glfwWindowShouldClose(_window->_handle))
	{
		float newTime = glfwGetTime();
		float deltaTime = newTime - _time;
		_time = newTime;

		Input::_Scroll = Vec2(0.0f, 0.0f);

		glfwPollEvents();

		_state->OnUpdate(deltaTime);
		_state->OnDraw();
	}
}

void Application::SwitchState(State* nextState)
{
	_Instance->_state->OnDestroy();
	delete _Instance->_state;
	_Instance->_state = nextState;
	_Instance->_state->OnCreate();
}
