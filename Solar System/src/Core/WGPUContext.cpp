#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

#include "WGPUContext.h"

using namespace wgpu;

WGPUContext::WGPUContext(
	GLFWwindow* window,
	uint32_t width,
	uint32_t height
)
:	instance(nullptr),
	surface(nullptr),
	adapter(nullptr),
	device(nullptr),
	swapChain(nullptr),
	swapChainFormat(TextureFormat::BGRA8Unorm),
	queue(nullptr)
{
	instance = createInstance(InstanceDescriptor{});
	if (!instance)
	{
		std::cerr << "Could not initialize WebGPU!" << std::endl;
		assert(false);
	}

	surface = glfwGetWGPUSurface(instance, window);
	RequestAdapterOptions adapterOpts;
	adapterOpts.compatibleSurface = surface;
	adapter = instance.requestAdapter(adapterOpts);

	SupportedLimits supportedLimits;
	adapter.getLimits(&supportedLimits);

	RequiredLimits requiredLimits = Default;
	requiredLimits.limits.maxVertexAttributes = 4;
	requiredLimits.limits.maxVertexBuffers = 2;
	requiredLimits.limits.maxBufferSize = 1000 * 1000 * 16; // 16MB
	requiredLimits.limits.maxVertexBufferArrayStride = 16 * sizeof(float);
	requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
	requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
	requiredLimits.limits.maxInterStageShaderComponents = 3;

	DeviceDescriptor deviceDesc;
	deviceDesc.label = "Device";
	deviceDesc.requiredFeaturesCount = 0;
	deviceDesc.requiredLimits = &requiredLimits;
	deviceDesc.defaultQueue.label = "Default Queue";
	device = adapter.requestDevice(deviceDesc);

	device.setUncapturedErrorCallback([](ErrorType type, char const* message)
	{
		std::cout << "Device error: type " << type;
		if (message) std::cout << " (message: " << message << ")";
		std::cout << std::endl;
	});

	SwapChainDescriptor swapChainDesc;
	swapChainDesc.width = width;
	swapChainDesc.height = height;
	swapChainDesc.usage = TextureUsage::RenderAttachment;
	swapChainDesc.format = swapChainFormat;
	swapChainDesc.presentMode = PresentMode::Fifo;
	swapChain = device.createSwapChain(surface, swapChainDesc);

	queue = device.getQueue();
}

WGPUContext::~WGPUContext()
{
	swapChain.release();
	device.release();
	adapter.release();
	surface.release();
	instance.release();
}
