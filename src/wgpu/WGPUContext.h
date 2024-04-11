#pragma once

#include <glfw3webgpu/glfw3webgpu.h>
#include <webgpu/webgpu.hpp>

#include "wgpu/DepthTexture.h"

class WGPUContext
{
public:
	WGPUContext(
		GLFWwindow* window,
		uint32_t width,
		uint32_t height
	);
	~WGPUContext();

	void StartFrame();
	void EndFrame();
public:
	wgpu::Instance instance = nullptr;
	wgpu::Surface surface = nullptr;
	wgpu::Adapter adapter = nullptr;
	wgpu::Device device = nullptr;
	wgpu::SwapChain swapChain = nullptr;
	wgpu::TextureFormat swapChainFormat = wgpu::TextureFormat::BGRA8Unorm;
	wgpu::Queue queue = nullptr;
	DepthTexture* depthTexture = nullptr;

	wgpu::TextureView nextTexture = nullptr;
	wgpu::CommandEncoder encoder = nullptr;
	wgpu::RenderPassEncoder renderPass = nullptr;
};
