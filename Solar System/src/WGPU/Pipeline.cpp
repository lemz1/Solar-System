#include "Pipeline.h"

#include "Core/Application.h"
#include "Util/AssetManager.h"

wgpu::ShaderModule loadShaderModule(
	const char* filePath,
	wgpu::Device device
)
{
	std::string shaderSource = AssetManager::GetFileContent(filePath);

	wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
	shaderCodeDesc.code = shaderSource.c_str();
	wgpu::ShaderModuleDescriptor shaderDesc;
	shaderDesc.nextInChain = &shaderCodeDesc.chain;
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;

	return device.createShaderModule(shaderDesc);
}

RenderPipeline::RenderPipeline(
	const char* shaderPath,
	const char* vertexEntryPoint,
	const char* fragmentEntryPoint
)
:	shaderModule(nullptr),
	pipeline(nullptr)
{
	wgpu::Device& device = Application::GetWGPUContext()->device;
	wgpu::TextureFormat& swapChainFormat = Application::GetWGPUContext()->swapChainFormat;

	shaderModule = loadShaderModule(shaderPath, device);

	wgpu::RenderPipelineDescriptor pipelineDesc;
	// Vertex fetch
	std::vector<wgpu::VertexAttribute> vertexAttribs(2);

	// Position attribute
	vertexAttribs[0].shaderLocation = 0;
	vertexAttribs[0].format = wgpu::VertexFormat::Float32x2;
	vertexAttribs[0].offset = 0;

	// Color attribute
	vertexAttribs[1].shaderLocation = 1;
	vertexAttribs[1].format = wgpu::VertexFormat::Float32x3;
	vertexAttribs[1].offset = 2 * sizeof(float);

	wgpu::VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.attributeCount = (uint32_t)vertexAttribs.size();
	vertexBufferLayout.attributes = vertexAttribs.data();
	vertexBufferLayout.arrayStride = 5 * sizeof(float);
	vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;

	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &vertexBufferLayout;

	pipelineDesc.vertex.module = shaderModule;
	pipelineDesc.vertex.entryPoint = vertexEntryPoint;
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
	pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
	pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;
	pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

	wgpu::FragmentState fragmentState;
	pipelineDesc.fragment = &fragmentState;
	fragmentState.module = shaderModule;
	fragmentState.entryPoint = fragmentEntryPoint;
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;

	wgpu::BlendState blendState;
	blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
	blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
	blendState.color.operation = wgpu::BlendOperation::Add;
	blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
	blendState.alpha.dstFactor = wgpu::BlendFactor::One;
	blendState.alpha.operation = wgpu::BlendOperation::Add;

	wgpu::ColorTargetState colorTarget;
	colorTarget.format = swapChainFormat;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = wgpu::ColorWriteMask::All;

	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;

	pipelineDesc.depthStencil = nullptr;

	pipelineDesc.multisample.count = 1;
	pipelineDesc.multisample.mask = ~0u;
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	wgpu::PipelineLayoutDescriptor layoutDesc;
	layoutDesc.bindGroupLayoutCount = 0;
	layoutDesc.bindGroupLayouts = nullptr;
	wgpu::PipelineLayout layout = device.createPipelineLayout(layoutDesc);
	pipelineDesc.layout = layout;

	pipeline = device.createRenderPipeline(pipelineDesc);
}

RenderPipeline::~RenderPipeline()
{
	pipeline.release();
	shaderModule.release();
}
