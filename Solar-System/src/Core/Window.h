#pragma once

#include <GLFW/glfw3.h>
#include <stdint.h>

class Window
{
public:
	Window(
		uint32_t width,
		uint32_t height,
		const char* title
	);
	~Window();

	uint32_t GetWidth() const { return _width; }
	uint32_t GetHeight() const { return _height; }
	const char* GetTitle() const { return _title; }
private:
	GLFWwindow* _handle;

	uint32_t _width;
	uint32_t _height;
	const char* _title;

	friend class Application;
};
