#include "PlanetCreator.h"

#include "Core/Application.h"
#include "Util/AssetManager.h"

using namespace wgpu;

void PlanetCreator::OnCreate()
{
	Device device = Application::GetWGPUContext()->device;

	_pipeline = new MeshPipeline("Assets/Shaders/shader.wgsl", "vs_main", "fs_main");

	_mesh = IcoSphere::Generate(3);

	_camera = new Camera(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());
	_camera->SetPosition(Vec3(0, 0, -3));

	_cameraController = new CameraController(_camera);

	TextureFormat depthTextureFormat = TextureFormat::Depth24Plus;
	TextureDescriptor depthTextureDesc;
	depthTextureDesc.dimension = TextureDimension::_2D;
	depthTextureDesc.format = depthTextureFormat;
	depthTextureDesc.mipLevelCount = 1;
	depthTextureDesc.sampleCount = 1;
	depthTextureDesc.size = { Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight(), 1 };
	depthTextureDesc.usage = TextureUsage::RenderAttachment;
	depthTextureDesc.viewFormatCount = 1;
	depthTextureDesc.viewFormats = (WGPUTextureFormat*)&depthTextureFormat;
	_depthTexture = device.createTexture(depthTextureDesc);

	TextureViewDescriptor depthTextureViewDesc;
	depthTextureViewDesc.aspect = TextureAspect::DepthOnly;
	depthTextureViewDesc.baseArrayLayer = 0;
	depthTextureViewDesc.arrayLayerCount = 1;
	depthTextureViewDesc.baseMipLevel = 0;
	depthTextureViewDesc.mipLevelCount = 1;
	depthTextureViewDesc.dimension = TextureViewDimension::_2D;
	depthTextureViewDesc.format = depthTextureFormat;
	_depthTextureView = _depthTexture.createView(depthTextureViewDesc);
}

void PlanetCreator::OnUpdate(float deltaTime)
{
	Queue queue = Application::GetWGPUContext()->queue;

	_cameraController->OnUpdate(deltaTime);

	MeshUniform uniform = {};
	uniform.modelViewProjection = _camera->GetProjection() * _camera->GetInverseView();

	queue.writeBuffer(_pipeline->uniformBuffer, 0, &uniform, sizeof(MeshUniform));
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
	depthStencilAttachment.view = _depthTextureView;
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

	renderPass.setPipeline(*_pipeline);
	renderPass.setBindGroup(0, _pipeline->bindGroup, 0, nullptr);

	_mesh->Draw(renderPass);

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
	_depthTextureView.release();
	_depthTexture.destroy();
	_depthTexture.release();
	delete _cameraController;
	delete _camera;
	delete _pipeline;
	delete _mesh;
}
