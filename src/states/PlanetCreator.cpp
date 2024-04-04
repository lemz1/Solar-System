#include "PlanetCreator.h"

#include "core/Application.h"
#include "util/AssetManager.h"
#include "core/Input.h"

using namespace wgpu;

void PlanetCreator::OnCreate()
{
	Device device = Application::GetWGPUContext()->device;

	_depthTexture = new DepthTexture(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_pipeline = new PlanetPipeline("assets/shaders/shader.wgsl", "vsMain", "fsMain");

	_planet = new Planet();

	_camera = new Camera(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_cameraController = new CameraController(_camera);}

void PlanetCreator::OnUpdate(float deltaTime)
{
	Queue queue = Application::GetWGPUContext()->queue;

	_cameraController->OnUpdate(deltaTime);

	PlanetUniform uniform = {};
	uniform.viewProjection = _camera->GetProjection() * _camera->GetInverseView();
	uniform.stScale = Vec2(1.0f);
	uniform.stTiling = Vec2(1.0f);
	uniform.nmScale = Vec2(1.0f);
	uniform.nmTiling = Vec2(1.0f);
	uniform.stBlendSharpness = 1.0f;
	uniform.nmBlendSharpness = 1.0f;

	queue.writeBuffer(_pipeline->uniformBuffer, 0, &uniform, sizeof(PlanetUniform));
}

void PlanetCreator::OnDraw()
{
	Instance instance = Application::GetWGPUContext()->instance;
	Surface surface = Application::GetWGPUContext()->surface;
	Adapter adapter = Application::GetWGPUContext()->adapter;
	Device device = Application::GetWGPUContext()->device;
	SwapChain swapChain = Application::GetWGPUContext()->swapChain;
	TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;
	Queue queue = Application::GetWGPUContext()->queue;

	TextureView nextTexture = swapChain.getCurrentTextureView();
	if (!nextTexture)
	{
		std::cerr << "Cannot acquire next swap chain texture" << std::endl;
		assert(false);
		return;
	}

	CommandEncoderDescriptor commandEncoderDesc;
	commandEncoderDesc.label = "Command Encoder";
	CommandEncoder encoder = device.createCommandEncoder(commandEncoderDesc);

	RenderPassDescriptor renderPassDesc;

	RenderPassColorAttachment renderPassColorAttachment{};
	renderPassColorAttachment.view = nextTexture;
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = LoadOp::Clear;
	renderPassColorAttachment.storeOp = StoreOp::Store;
	renderPassColorAttachment.clearValue = Color{ 0.1, 0.1, 0.1, 1.0 };

	RenderPassDepthStencilAttachment depthStencilAttachment;
	depthStencilAttachment.view = _depthTexture->GetTextureView();
	depthStencilAttachment.depthClearValue = 1.0f;
	depthStencilAttachment.depthLoadOp = LoadOp::Clear;
	depthStencilAttachment.depthStoreOp = StoreOp::Store;
	depthStencilAttachment.depthReadOnly = false;
	depthStencilAttachment.stencilClearValue = 0;
	depthStencilAttachment.stencilLoadOp = LoadOp::Clear;
	depthStencilAttachment.stencilStoreOp = StoreOp::Store;
	depthStencilAttachment.stencilReadOnly = true;

	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;
	renderPassDesc.depthStencilAttachment = &depthStencilAttachment;
	renderPassDesc.timestampWriteCount = 0;
	renderPassDesc.timestampWrites = nullptr;
	renderPassDesc.label = "Render Pass";

	RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);

	renderPass.setPipeline(_pipeline->pipeline);
	renderPass.setBindGroup(0, _pipeline->bindGroup, 0, nullptr);

	_planet->GetMesh()->Draw(renderPass);

	renderPass.end();
	renderPass.release();

	nextTexture.release();

	CommandBufferDescriptor cmdBufferDescriptor;
	cmdBufferDescriptor.label = "Command buffer";
	CommandBuffer command = encoder.finish(cmdBufferDescriptor);
	encoder.release();
	queue.submit(command);
	command.release();

	swapChain.present();
}

void PlanetCreator::OnDestroy()
{
	delete _cameraController;
	delete _camera;
	delete _planet;
	delete _pipeline;
	delete _depthTexture;
}
