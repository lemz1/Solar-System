#include "MoonCreator.h"

#include "core/Application.h"
#include "util/AssetManager.h"
#include "core/Input.h"
#include "celestial/MoonComputePipeline.h"
#include "debug/Timer.h"

#include <functional>

void MoonCreator::OnCreate()
{
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	_depthTexture = new DepthTexture(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_moon = new Moon();

	_camera = new Camera(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_cameraController = new CameraController(_camera);
}

void MoonCreator::OnEvent(const Event& event)
{
	Event::Dispatch<WindowResizeEvent>(event, BindEventFunction(MoonCreator::OnResize));
}

void MoonCreator::OnUpdate(float deltaTime)
{
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	_cameraController->OnUpdate(deltaTime);

	Mat4 viewProjection = _camera->GetProjection() * _camera->GetInverseView();

	queue.writeBuffer(_moon->GetRenderPipeline()->uniformBuffer, offsetof(MoonRenderUniform, viewProjection), &viewProjection, sizeof(Mat4));
}

void MoonCreator::OnDraw()
{
	wgpu::Instance instance = Application::GetWGPUContext()->instance;
	wgpu::Surface surface = Application::GetWGPUContext()->surface;
	wgpu::Adapter adapter = Application::GetWGPUContext()->adapter;
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::SwapChain swapChain = Application::GetWGPUContext()->swapChain;
	wgpu::TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	wgpu::TextureView nextTexture = swapChain.getCurrentTextureView();
	if (!nextTexture)
	{
		std::cerr << "Cannot acquire next swap chain texture" << std::endl;
		assert(false);
		return;
	}

	wgpu::CommandEncoderDescriptor commandEncoderDesc = wgpu::Default;
	commandEncoderDesc.label = "Command Encoder";
	wgpu::CommandEncoder encoder = device.createCommandEncoder(commandEncoderDesc);

	wgpu::RenderPassDescriptor renderPassDesc = wgpu::Default;

	wgpu::RenderPassColorAttachment renderPassColorAttachment = wgpu::Default;
	renderPassColorAttachment.view = nextTexture;
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
	renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
	renderPassColorAttachment.clearValue = wgpu::Color{ 0.1, 0.1, 0.1, 1.0 };

	wgpu::RenderPassDepthStencilAttachment depthStencilAttachment = wgpu::Default;
	depthStencilAttachment.view = _depthTexture->GetTextureView();
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

	wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);

	_moon->Draw(renderPass);

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

void MoonCreator::OnDestroy()
{
	delete _cameraController;
	delete _camera;
	delete _moon;
	delete _depthTexture;
}

void MoonCreator::OnResize(const WindowResizeEvent &event)
{
	_camera->Resize(event.GetWidth(), event.GetHeight());

	_depthTexture->Resize(event.GetWidth(), event.GetHeight());
}
