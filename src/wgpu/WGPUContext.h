#pragma once

#include <glfw3webgpu/glfw3webgpu.h>
#include <webgpu/webgpu.hpp>

class WGPUContext
{
public:
	WGPUContext(
		GLFWwindow* window,
		uint32_t width,
		uint32_t height
	);
	~WGPUContext();
public:
	wgpu::Instance instance;
	wgpu::Surface surface;
	wgpu::Adapter adapter;
	wgpu::Device device;
	wgpu::SwapChain swapChain;
	wgpu::TextureFormat swapChainFormat;
	wgpu::Queue queue;
};
