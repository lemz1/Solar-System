#include "WGPUContext.h"

WGPUContext::WGPUContext(
	GLFWwindow* window,
	uint32_t width,
	uint32_t height
)
{
	instance = createInstance(wgpu::InstanceDescriptor{});
	if (!instance)
	{
		std::cerr << "Could not initialize WebGPU!" << std::endl;
		assert(false);
	}

	surface = glfwGetWGPUSurface(instance, window);
	wgpu::RequestAdapterOptions adapterOpts;
	adapterOpts.compatibleSurface = surface;
	adapter = instance.requestAdapter(adapterOpts);

	wgpu::SupportedLimits supportedLimits;
	adapter.getLimits(&supportedLimits);

	wgpu::RequiredLimits requiredLimits = wgpu::Default;
	requiredLimits.limits.maxVertexAttributes = 3;
	requiredLimits.limits.maxVertexBuffers = 3;
	requiredLimits.limits.maxBufferSize = 1000 * 1000 * 16; // 16MB
	requiredLimits.limits.maxVertexBufferArrayStride = 16 * sizeof(float);
	requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
	requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
	requiredLimits.limits.maxInterStageShaderComponents = 6;
	requiredLimits.limits.maxBindGroups = 2;
	requiredLimits.limits.maxUniformBuffersPerShaderStage = 1;
	requiredLimits.limits.maxUniformBufferBindingSize = 64 * sizeof(float);
	requiredLimits.limits.maxTextureDimension1D = 2160;
	requiredLimits.limits.maxTextureDimension2D = 3840;
	requiredLimits.limits.maxTextureArrayLayers = 1;
	requiredLimits.limits.maxSampledTexturesPerShaderStage = 1;
	requiredLimits.limits.maxSamplersPerShaderStage = 1;
	requiredLimits.limits.maxStorageBuffersPerShaderStage = 2;
	requiredLimits.limits.maxStorageBufferBindingSize = 1000 * 1000 * 16; // 16MB;
	requiredLimits.limits.maxComputeWorkgroupSizeX = 128;
	requiredLimits.limits.maxComputeWorkgroupSizeY = 1;
	requiredLimits.limits.maxComputeWorkgroupSizeZ = 1;
	requiredLimits.limits.maxComputeInvocationsPerWorkgroup = 128;
	requiredLimits.limits.maxComputeWorkgroupsPerDimension = 512;
	
	wgpu::DeviceDescriptor deviceDesc;
	deviceDesc.label = "Device";
	deviceDesc.requiredFeaturesCount = 0;
	deviceDesc.requiredLimits = &requiredLimits;
	deviceDesc.defaultQueue.label = "Default Queue";
	device = adapter.requestDevice(deviceDesc);

	device.getLimits(&supportedLimits);

	// I am using C-Style function here because
	// I dont want to store the call back
	// in a unique pointer
	auto onDeviceError = [](WGPUErrorType type, char const* message, void*)
	{
		std::cout << "Uncaptured device error: type " << type;
		if (message) std::cout << " (" << message << ")";
		std::cout << std::endl;
	};
	wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr);

	swapChainFormat = surface.getPreferredFormat(adapter);

	wgpu::SwapChainDescriptor swapChainDesc;
	swapChainDesc.label = "SwapChain";
	swapChainDesc.width = width;
	swapChainDesc.height = height;
	swapChainDesc.usage = wgpu::TextureUsage::RenderAttachment;
	swapChainDesc.format = swapChainFormat;
	swapChainDesc.presentMode = wgpu::PresentMode::Fifo;
	swapChain = device.createSwapChain(surface, swapChainDesc);

	queue = device.getQueue();

	depthTexture = new DepthTexture(device, width, height);
}

WGPUContext::~WGPUContext()
{
	swapChain.release();
	device.release();
	adapter.release();
	surface.release();
	instance.release();
}

void WGPUContext::StartFrame()
{
	nextTexture = swapChain.getCurrentTextureView();
	if (!nextTexture)
	{
		std::cerr << "Cannot acquire next swap chain texture" << std::endl;
		assert(false);
		return;
	}

	wgpu::CommandEncoderDescriptor commandEncoderDesc = wgpu::Default;
	commandEncoderDesc.label = "Command Encoder";
	encoder = device.createCommandEncoder(commandEncoderDesc);

	wgpu::RenderPassDescriptor renderPassDesc = wgpu::Default;

	wgpu::RenderPassColorAttachment renderPassColorAttachment = wgpu::Default;
	renderPassColorAttachment.view = nextTexture;
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
	renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
	renderPassColorAttachment.clearValue = wgpu::Color{ 0.1, 0.1, 0.1, 1.0 };

	wgpu::RenderPassDepthStencilAttachment depthStencilAttachment = wgpu::Default;
	depthStencilAttachment.view = depthTexture->GetTextureView();
	depthStencilAttachment.depthClearValue = 1.0f;
	depthStencilAttachment.depthLoadOp = wgpu::LoadOp::Clear;
	depthStencilAttachment.depthStoreOp = wgpu::StoreOp::Store;
	depthStencilAttachment.depthReadOnly = false;
	depthStencilAttachment.stencilClearValue = 0;
	depthStencilAttachment.stencilLoadOp = wgpu::LoadOp::Clear;
	depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Store;
	depthStencilAttachment.stencilReadOnly = true;

	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;
	renderPassDesc.depthStencilAttachment = &depthStencilAttachment;
	renderPassDesc.timestampWriteCount = 0;
	renderPassDesc.timestampWrites = nullptr;
	renderPassDesc.label = "Render Pass";

	renderPass = encoder.beginRenderPass(renderPassDesc);
}

void WGPUContext::EndFrame()
{
	renderPass.end();
	renderPass.release();

	nextTexture.release();

	wgpu::CommandBufferDescriptor cmdBufferDescriptor = wgpu::Default;
	cmdBufferDescriptor.label = "Command buffer";
	wgpu::CommandBuffer commands = encoder.finish(cmdBufferDescriptor);
	encoder.release();
	queue.submit(commands);
	commands.release();

	swapChain.present();
}
