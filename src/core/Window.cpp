#include "Window.h"

#include <iostream>
#include <cassert>

Window::Window(
	uint32_t width, 
	uint32_t height, 
	const char* title
)
:	_width(width),
	_height(height),
	_title(title)
{
	if (!glfwInit())
	{
		std::cerr << "Could not initialize GLFW!" << std::endl;
		assert(false);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!_handle)
	{
		glfwTerminate();
		std::cerr << "Could not create window!" << std::endl;
		assert(false);
	}
}

Window::~Window()
{
	glfwDestroyWindow(_handle);
	glfwTerminate();
}
