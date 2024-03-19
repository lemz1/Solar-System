#include "Application.h"

Application* Application::_instance = nullptr;

Application::Application(
	uint32_t width, 
	uint32_t height, 
	const char* title,
	State* initialState
)
{
	if (_instance)
	{
		return;
	}

	_instance = this;

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
	float time = 0;

	while (!glfwWindowShouldClose(_window->_handle))
	{
		float newTime = glfwGetTime();
		float deltaTime = newTime - time;
		time = newTime;

		glfwPollEvents();

		_state->OnUpdate(deltaTime);
		_state->OnDraw();
	}
}

void Application::SwitchState(State* nextState)
{
	_instance->_state->OnDestroy();
	delete _instance->_state;
	_instance->_state = nextState;
	_instance->_state->OnCreate();
}
