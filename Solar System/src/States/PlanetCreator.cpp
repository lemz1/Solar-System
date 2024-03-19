#include "PlanetCreator.h"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "Core/Application.h"
#include "Util/AssetManager.h"

namespace fs = std::filesystem;

bool loadGeometry(
	const char* filePath, 
	std::vector<float>& pointData, 
	std::vector<uint16_t>& indexData
);

void PlanetCreator::OnCreate()
{
	wgpu::Instance instance = Application::GetWGPUContext()->instance;
	wgpu::Surface surface = Application::GetWGPUContext()->surface;
	wgpu::Adapter adapter = Application::GetWGPUContext()->adapter;
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::SwapChain swapChain = Application::GetWGPUContext()->swapChain;
	wgpu::TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	pipeline = new RenderPipeline("Assets/Shaders/shader.wgsl", "vs_main", "fs_main");

	bool success = loadGeometry("Assets/Shaders/webgpu.txt", pointData, indexData);
	if (!success)
	{
		std::cerr << "Could not load geometry!" << std::endl;
		assert(false);
		return;
	}

	wgpu::BufferDescriptor bufferDesc;
	bufferDesc.size = pointData.size() * sizeof(float);
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
	bufferDesc.mappedAtCreation = false;
	vertexBuffer = device.createBuffer(bufferDesc);
	queue.writeBuffer(vertexBuffer, 0, pointData.data(), bufferDesc.size);

	bufferDesc.size = indexData.size() * sizeof(float);
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Index;
	bufferDesc.mappedAtCreation = false;
	indexBuffer = device.createBuffer(bufferDesc);
	queue.writeBuffer(indexBuffer, 0, indexData.data(), bufferDesc.size);
}

void PlanetCreator::OnUpdate(float deltaTime)
{}

void PlanetCreator::OnDraw()
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

	wgpu::CommandEncoderDescriptor commandEncoderDesc;
	commandEncoderDesc.label = "Command Encoder";
	wgpu::CommandEncoder encoder = device.createCommandEncoder(commandEncoderDesc);

	wgpu::RenderPassDescriptor renderPassDesc;

	wgpu::RenderPassColorAttachment renderPassColorAttachment{};
	renderPassColorAttachment.view = nextTexture;
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
	renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
	renderPassColorAttachment.clearValue = wgpu::Color{ 0.05, 0.05, 0.05, 1.0 };
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;

	renderPassDesc.depthStencilAttachment = nullptr;
	renderPassDesc.timestampWriteCount = 0;
	renderPassDesc.timestampWrites = nullptr;
	wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);

	renderPass.setPipeline(pipeline->pipeline);

	renderPass.setVertexBuffer(0, vertexBuffer, 0, pointData.size() * sizeof(float));
	renderPass.setIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint16, 0, indexData.size() * sizeof(uint16_t));

	renderPass.drawIndexed(indexData.size(), 1, 0, 0, 0);

	renderPass.end();
	renderPass.release();

	nextTexture.release();

	wgpu::CommandBufferDescriptor cmdBufferDescriptor;
	cmdBufferDescriptor.label = "Command buffer";
	wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
	encoder.release();
	queue.submit(command);
	command.release();

	swapChain.present();
}

void PlanetCreator::OnDestroy()
{
	vertexBuffer.destroy();
	vertexBuffer.release();
	indexBuffer.destroy();
	indexBuffer.release();

	delete pipeline;
}

bool loadGeometry(
	const char* filePath, 
	std::vector<float>& pointData, 
	std::vector<uint16_t>& indexData
)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return false;
	}

	pointData.clear();
	indexData.clear();

	enum class Section
	{
		None,
		Points,
		Indices,
	};
	Section currentSection = Section::None;

	float value;
	uint16_t index;
	std::string line;
	while (!file.eof())
	{
		getline(file, line);
		if (line == "[points]")
		{
			currentSection = Section::Points;
		}
		else if (line == "[indices]")
		{
			currentSection = Section::Indices;
		}
		else if (line[0] == '#' || line.empty())
		{
			// Do nothing, this is a comment
		}
		else if (currentSection == Section::Points)
		{
			std::istringstream iss(line);
			// Get x, y, r, g, b
			for (int i = 0; i < 5; ++i)
			{
				iss >> value;
				pointData.push_back(value);
			}
		}
		else if (currentSection == Section::Indices)
		{
			std::istringstream iss(line);
			// Get corners #0 #1 and #2
			for (int i = 0; i < 3; ++i)
			{
				iss >> index;
				indexData.push_back(index);
			}
		}
	}
	return true;
}