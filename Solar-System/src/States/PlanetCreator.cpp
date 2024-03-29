#include "PlanetCreator.h"

#include "Core/Application.h"
#include "Util/AssetManager.h"

using namespace wgpu;

void PlanetCreator::OnCreate()
{
	_pipeline = new MeshPipeline("Assets/Shaders/shader.wgsl", "vs_main", "fs_main");

	std::vector<Vec3> vertices = {
		Vec3(-1,  1,  0),
		Vec3( 1,  1,  0),
		Vec3(-1, -1,  0),
		Vec3( 1, -1,  0),
	};

	std::vector<Vec2> texCoords = {
		Vec2(0, 1),
		Vec2(1, 1),
		Vec2(0, 0),
		Vec2(1, 0),
	};

	std::vector<Vec3> normals = {
		Vec3( 0,  0, -1),
		Vec3( 0,  0, -1),
		Vec3( 0,  0, -1),
		Vec3( 0,  0, -1),
	};

	std::vector<uint32_t> indices = {
		0, 2, 1,
		1, 3, 2
	};

	_mesh = new Mesh(vertices, texCoords, normals, indices, {});

	//_mesh = IcoSphere::Generate(1);

	_camera = new Camera(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_cameraController = new CameraController(_camera);
}

void PlanetCreator::OnUpdate(float deltaTime)
{
	Queue queue = Application::GetWGPUContext()->queue;

	MeshUniform uniform = {};
	uniform.uTime = Application::GetTime();
	queue.writeBuffer(_pipeline->uniformBuffer, 0, &uniform, sizeof(MeshUniform));

	_cameraController->OnUpdate(deltaTime);
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
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;

	renderPassDesc.depthStencilAttachment = nullptr;
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
	delete _cameraController;
	delete _camera;
	delete _pipeline;
	delete _mesh;
}
