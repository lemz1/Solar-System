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

	_wgpuContext = new WGPUContext(_window->GetHandle(), _window->GetWidth(), _window->GetHeight());

	_myguiContext = new MyGuiContext();

	_state = initialState;
	_state->OnCreate();
}

Application::~Application()
{
	_state->OnDestroy();
	delete _state;

	delete _myguiContext;
	delete _wgpuContext;
	delete _window;
}

void Application::Run()
{
	_time = 0;

	while (!glfwWindowShouldClose(_window->GetHandle()))
	{
		float newTime = (float)glfwGetTime();
		float deltaTime = newTime - _time;
		_time = newTime;

		Input::_Scroll = Vec2(0.0f, 0.0f);

		glfwPollEvents();

		_state->OnUpdate(deltaTime);

		_wgpuContext->StartFrame();

		_state->OnDraw(_wgpuContext->renderPass);

		_myguiContext->StartFrame();
		_state->OnDrawImGui();
		_myguiContext->EndFrame();

		_wgpuContext->EndFrame();
	}
}

void Application::SwitchState(State* nextState)
{
	_Instance->_state->OnDestroy();
	delete _Instance->_state;
	_Instance->_state = nextState;
	_Instance->_state->OnCreate();
}
