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

	GLFWwindow* GetHandle() { return _handle; }
	uint32_t GetWidth() const { return _data.width; }
	uint32_t GetHeight() const { return _data.height; }
	const char* GetTitle() const { return _data.title; }
private:
	GLFWwindow* _handle;

	struct WindowData 
	{
		uint32_t width;
		uint32_t height;
		const char* title;
	};

	WindowData _data;
};
