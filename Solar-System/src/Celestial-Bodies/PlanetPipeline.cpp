#include "PlanetPipeline.h"

#include "Core/Application.h"
#include "Util/AssetManager.h"
#include "WGPU/DepthTexture.h"

using namespace wgpu;

PlanetPipeline::PlanetPipeline(
	const char* shaderPath,
	const char* vertexEntryPoint,
	const char* fragmentEntryPoint
)
:	shaderModule(nullptr),
	bindGroupLayout(nullptr),
	surfaceTexture(nullptr),
	normalMap(nullptr),
	uniformBuffer(nullptr),
	bindGroup(nullptr),
	layout(nullptr),
	pipeline(nullptr)
{
	Device device = Application::GetWGPUContext()->device;
	Queue queue = Application::GetWGPUContext()->queue;
	TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;

	shaderModule = AssetManager::LoadShaderModule(shaderPath);

	surfaceTexture = AssetManager::LoadTexture2D("Assets/Images/Wall_Tiles_Stone_001_basecolor.jpg", TextureFormat::RGBA8Unorm);
	normalMap = AssetManager::LoadTexture2D("Assets/Images/Wall_Tiles_Stone_001_normal.jpg", TextureFormat::RGBA8Unorm);

	RenderPipelineDescriptor pipelineDesc;
	pipelineDesc.label = "Planet Pipeline";

	// Vertex State
	std::vector<VertexAttribute> vertexAttribs(2);

	{ // Position attribute
		VertexAttribute& vertexAttribute = vertexAttribs[0];
		vertexAttribute.shaderLocation = 0;
		vertexAttribute.format = VertexFormat::Float32x3;
		vertexAttribute.offset = 0;
	}

	{ // Normal attribute
		VertexAttribute& vertexAttribute = vertexAttribs[1];
		vertexAttribute.shaderLocation = 1;
		vertexAttribute.format = VertexFormat::Float32x3;
		vertexAttribute.offset = 0;
	}

	std::vector<VertexBufferLayout> vertexBufferLayouts(2);

	{ // Position attribute
		VertexBufferLayout& vertexBufferLayout = vertexBufferLayouts[0];
		vertexBufferLayout.attributeCount = 1;
		vertexBufferLayout.attributes = &vertexAttribs[0];
		vertexBufferLayout.arrayStride = 3 * sizeof(float);
		vertexBufferLayout.stepMode = VertexStepMode::Vertex;
	}

	{ // Normal attribute
		VertexBufferLayout& vertexBufferLayout = vertexBufferLayouts[1];
		vertexBufferLayout.attributeCount = 1;
		vertexBufferLayout.attributes = &vertexAttribs[1];
		vertexBufferLayout.arrayStride = 3 * sizeof(float);
		vertexBufferLayout.stepMode = VertexStepMode::Vertex;
	}

	pipelineDesc.vertex.bufferCount = (uint32_t)vertexBufferLayouts.size();
	pipelineDesc.vertex.buffers = vertexBufferLayouts.data();

	pipelineDesc.vertex.module = shaderModule;
	pipelineDesc.vertex.entryPoint = vertexEntryPoint;
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = PrimitiveTopology::TriangleList;
	pipelineDesc.primitive.stripIndexFormat = IndexFormat::Undefined;
	pipelineDesc.primitive.frontFace = FrontFace::CCW;
	pipelineDesc.primitive.cullMode = CullMode::Back;

	// Fragment State
	FragmentState fragmentState;
	fragmentState.module = shaderModule;
	fragmentState.entryPoint = fragmentEntryPoint;
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;

	BlendState blendState;
	blendState.color.srcFactor = BlendFactor::SrcAlpha;
	blendState.color.dstFactor = BlendFactor::OneMinusSrcAlpha;
	blendState.color.operation = BlendOperation::Add;
	blendState.alpha.srcFactor = BlendFactor::Zero;
	blendState.alpha.dstFactor = BlendFactor::One;
	blendState.alpha.operation = BlendOperation::Add;

	ColorTargetState colorTarget;
	colorTarget.format = swapChainFormat;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = ColorWriteMask::All;

	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;

	pipelineDesc.fragment = &fragmentState;

	DepthStencilState depthStencilState = Default;
	depthStencilState.depthCompare = CompareFunction::Less;
	depthStencilState.depthWriteEnabled = true;
	depthStencilState.format = DepthTexture::GetTextureFormat();
	depthStencilState.stencilReadMask = 0;
	depthStencilState.stencilWriteMask = 0;

	pipelineDesc.depthStencil = &depthStencilState;

	pipelineDesc.multisample.count = 1;
	pipelineDesc.multisample.mask = ~0u;
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	// Bind Groups (Uniforms)
	std::vector<BindGroupLayoutEntry> bindingLayoutEntries(3);

	{
		BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[0];
		bindingLayout.binding = 0;
		bindingLayout.visibility = ShaderStage::Vertex;
		bindingLayout.buffer.type = BufferBindingType::Uniform;
		bindingLayout.buffer.minBindingSize = sizeof(PlanetUniform);
	}

	{
		BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[1];
		bindingLayout.binding = 1;
		bindingLayout.visibility = ShaderStage::Fragment;
		bindingLayout.texture.sampleType = TextureSampleType::Float;
		bindingLayout.texture.viewDimension = TextureViewDimension::_2D;
	}

	{
		BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[2];
		bindingLayout.binding = 2;
		bindingLayout.visibility = ShaderStage::Fragment;
		bindingLayout.texture.sampleType = TextureSampleType::Float;
		bindingLayout.texture.viewDimension = TextureViewDimension::_2D;
	}

	BindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.entryCount = bindingLayoutEntries.size();
	bindGroupLayoutDesc.entries = bindingLayoutEntries.data();
	bindGroupLayout = device.createBindGroupLayout(bindGroupLayoutDesc);

	BufferDescriptor bufferDesc{};
	bufferDesc.size = sizeof(PlanetUniform);
	bufferDesc.usage = BufferUsage::CopyDst | BufferUsage::Uniform;
	bufferDesc.mappedAtCreation = false;
	uniformBuffer = device.createBuffer(bufferDesc);

	PlanetUniform uniform = {};
	queue.writeBuffer(uniformBuffer, 0, &uniform, sizeof(PlanetUniform));

	std::vector<BindGroupEntry> bindings(3);

	{
		BindGroupEntry& binding = bindings[0];
		binding.binding = 0;
		binding.offset = 0;
		binding.buffer = uniformBuffer;
		binding.size = sizeof(PlanetUniform);
	}

	{
		BindGroupEntry& binding = bindings[1];
		binding.binding = 1;
		binding.textureView = surfaceTexture->GetTextureView();
	}

	{
		BindGroupEntry& binding = bindings[2];
		binding.binding = 2;
		binding.textureView = normalMap->GetTextureView();
	}

	BindGroupDescriptor bindGroupDesc{};
	bindGroupDesc.layout = bindGroupLayout;
	bindGroupDesc.entryCount = bindings.size();
	bindGroupDesc.entries = bindings.data();
	bindGroup = device.createBindGroup(bindGroupDesc);

	PipelineLayoutDescriptor layoutDesc;
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*)&bindGroupLayout;
	layout = device.createPipelineLayout(layoutDesc);
	pipelineDesc.layout = layout;

	pipeline = device.createRenderPipeline(pipelineDesc);
}

PlanetPipeline::~PlanetPipeline()
{
	pipeline.release();
	layout.release();
	bindGroup.release();
	uniformBuffer.destroy();
	delete surfaceTexture;
	delete normalMap;
	uniformBuffer.release();
	bindGroupLayout.release();
	shaderModule.release();
}
