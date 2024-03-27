#include "MeshPipeline.h"

#include "Core/Application.h"
#include "Util/AssetManager.h"

using namespace wgpu;

MeshPipeline::MeshPipeline(
	const char* shaderPath,
	const char* vertexEntryPoint,
	const char* fragmentEntryPoint
)
:	shaderModule(nullptr),
	pipeline(nullptr)
{
	Device device = Application::GetWGPUContext()->device;
	TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;

	shaderModule = AssetManager::LoadShaderModule(shaderPath);

	RenderPipelineDescriptor pipelineDesc;
	pipelineDesc.label = "Mesh Pipeline";
	// Vertex fetch
	std::vector<VertexAttribute> vertexAttribs(3);

	// Position attribute
	vertexAttribs[0].shaderLocation = 0;
	vertexAttribs[0].format = VertexFormat::Float32x3;
	vertexAttribs[0].offset = 0;

	// TexCoord attribute
	vertexAttribs[1].shaderLocation = 1;
	vertexAttribs[1].format = VertexFormat::Float32x2;
	vertexAttribs[1].offset = 0;

	// Normal attribute
	vertexAttribs[2].shaderLocation = 2;
	vertexAttribs[2].format = VertexFormat::Float32x3;
	vertexAttribs[2].offset = 0;

	std::vector<VertexBufferLayout> vertexBufferLayouts(3);
	vertexBufferLayouts[0].attributeCount = 1;
	vertexBufferLayouts[0].attributes = &vertexAttribs.at(0);
	vertexBufferLayouts[0].arrayStride = 3 * sizeof(float);
	vertexBufferLayouts[0].stepMode = VertexStepMode::Vertex;

	vertexBufferLayouts[1].attributeCount = 1;
	vertexBufferLayouts[1].attributes = &vertexAttribs.at(1);
	vertexBufferLayouts[1].arrayStride = 2 * sizeof(float);
	vertexBufferLayouts[1].stepMode = VertexStepMode::Vertex;

	vertexBufferLayouts[2].attributeCount = 1;
	vertexBufferLayouts[2].attributes = &vertexAttribs.at(2);
	vertexBufferLayouts[2].arrayStride = 3 * sizeof(float);
	vertexBufferLayouts[2].stepMode = VertexStepMode::Vertex;

	pipelineDesc.vertex.bufferCount = (uint32_t)vertexBufferLayouts.size();
	pipelineDesc.vertex.buffers = vertexBufferLayouts.data();

	pipelineDesc.vertex.module = shaderModule;
	pipelineDesc.vertex.entryPoint = vertexEntryPoint;
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = PrimitiveTopology::TriangleList;
	pipelineDesc.primitive.stripIndexFormat = IndexFormat::Undefined;
	pipelineDesc.primitive.frontFace = FrontFace::CCW;
	pipelineDesc.primitive.cullMode = CullMode::None;

	FragmentState fragmentState;
	pipelineDesc.fragment = &fragmentState;
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

	pipelineDesc.depthStencil = nullptr;

	pipelineDesc.multisample.count = 1;
	pipelineDesc.multisample.mask = ~0u;
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	PipelineLayoutDescriptor layoutDesc;
	layoutDesc.bindGroupLayoutCount = 0;
	layoutDesc.bindGroupLayouts = nullptr;
	PipelineLayout layout = device.createPipelineLayout(layoutDesc);
	pipelineDesc.layout = layout;

	pipeline = device.createRenderPipeline(pipelineDesc);
}

MeshPipeline::~MeshPipeline()
{
	pipeline.release();
	shaderModule.release();
}
