#include "MoonRenderPipeline.h"

#include "core/Application.h"
#include "util/AssetManager.h"
#include "wgpu/DepthTexture.h"

MoonRenderPipeline::MoonRenderPipeline(const char* shaderPath)
{
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;
	wgpu::TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;

	shaderModule = AssetManager::LoadShaderModule(shaderPath);

	wgpu::RenderPipelineDescriptor pipelineDesc;
	pipelineDesc.label = "Moon Render Pipeline";

	// Vertex State
	Vector<wgpu::VertexAttribute> vertexAttribs(2, wgpu::Default);

	{ // Position attribute
		wgpu::VertexAttribute& vertexAttribute = vertexAttribs[0];
		vertexAttribute.shaderLocation = 0;
		vertexAttribute.format = wgpu::VertexFormat::Float32x3;
		vertexAttribute.offset = 0;
	}

	{ // Normal attribute
		wgpu::VertexAttribute& vertexAttribute = vertexAttribs[1];
		vertexAttribute.shaderLocation = 1;
		vertexAttribute.format = wgpu::VertexFormat::Float32x3;
		vertexAttribute.offset = 0;
	}

	Vector<wgpu::VertexBufferLayout> vertexBufferLayouts(2, wgpu::Default);

	{ // Position attribute
		wgpu::VertexBufferLayout& vertexBufferLayout = vertexBufferLayouts[0];
		vertexBufferLayout.attributeCount = 1;
		vertexBufferLayout.attributes = &vertexAttribs[0];
		vertexBufferLayout.arrayStride = sizeof(Vec3);
		vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;
	}

	{ // Normal attribute
		wgpu::VertexBufferLayout& vertexBufferLayout = vertexBufferLayouts[1];
		vertexBufferLayout.attributeCount = 1;
		vertexBufferLayout.attributes = &vertexAttribs[1];
		vertexBufferLayout.arrayStride = sizeof(Vec3);
		vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;
	}

	pipelineDesc.vertex.bufferCount = (uint32_t)vertexBufferLayouts.size();
	pipelineDesc.vertex.buffers = vertexBufferLayouts.data();

	pipelineDesc.vertex.module = shaderModule;
	pipelineDesc.vertex.entryPoint = "vsMain";
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;

	pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
	pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
	pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;
	pipelineDesc.primitive.cullMode = wgpu::CullMode::Back;

	// Fragment State
	wgpu::FragmentState fragmentState;
	fragmentState.module = shaderModule;
	fragmentState.entryPoint = "fsMain";
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

	pipelineDesc.fragment = &fragmentState;

	wgpu::DepthStencilState depthStencilState = wgpu::Default;
	depthStencilState.depthCompare = wgpu::CompareFunction::Less;
	depthStencilState.depthWriteEnabled = true;
	depthStencilState.format = DepthTexture::GetTextureFormat();
	depthStencilState.stencilReadMask = 0;
	depthStencilState.stencilWriteMask = 0;

	pipelineDesc.depthStencil = &depthStencilState;

	pipelineDesc.multisample.count = 1;
	pipelineDesc.multisample.mask = ~0u;
	pipelineDesc.multisample.alphaToCoverageEnabled = false;

	// Bind Groups (Uniforms)
	Vector<wgpu::BindGroupLayoutEntry> bindingLayoutEntries(3, wgpu::Default);

	{
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[0];
		bindingLayout.binding = 0;
		bindingLayout.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
		bindingLayout.buffer.type = wgpu::BufferBindingType::Uniform;
		bindingLayout.buffer.minBindingSize = sizeof(MoonRenderUniform);
	}

	{
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[1];
		bindingLayout.binding = 1;
		bindingLayout.visibility = wgpu::ShaderStage::Fragment;
		bindingLayout.sampler.type = wgpu::SamplerBindingType::Filtering;
	}

	{
		wgpu::BindGroupLayoutEntry& bindingLayout = bindingLayoutEntries[2];
		bindingLayout.binding = 2;
		bindingLayout.visibility = wgpu::ShaderStage::Fragment;
		bindingLayout.texture.sampleType = wgpu::TextureSampleType::Float;
		bindingLayout.texture.viewDimension = wgpu::TextureViewDimension::_2D;
	}

	wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.entryCount = (uint32_t)bindingLayoutEntries.size();
	bindGroupLayoutDesc.entries = bindingLayoutEntries.data();
	bindGroupLayout = device.createBindGroupLayout(bindGroupLayoutDesc);

	wgpu::BufferDescriptor bufferDesc{};
	bufferDesc.size = sizeof(MoonRenderUniform);
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
	bufferDesc.mappedAtCreation = false;
	uniformBuffer = device.createBuffer(bufferDesc);

	MoonRenderUniform uniform = {};
	uniform.normalMapScale = Vec2(0.5f);
	uniform.normalMapTiling = Vec2(1.0f);
	uniform.normalMapBlendSharpness = 16.0f;
	uniform.moonColor = Vec3(0.3f);

	queue.writeBuffer(uniformBuffer, 0, &uniform, sizeof(MoonRenderUniform));
	
	wgpu::SamplerDescriptor samplerDesc;
	samplerDesc.addressModeU = wgpu::AddressMode::Repeat;
	samplerDesc.addressModeV = wgpu::AddressMode::Repeat;
	samplerDesc.addressModeW = wgpu::AddressMode::Repeat;
	samplerDesc.magFilter = wgpu::FilterMode::Linear;
	samplerDesc.minFilter = wgpu::FilterMode::Linear;
	samplerDesc.mipmapFilter = wgpu::MipmapFilterMode::Linear;
	samplerDesc.lodMinClamp = 0.0f;
	samplerDesc.lodMaxClamp = 1.0f;
	samplerDesc.compare = wgpu::CompareFunction::Undefined;
	samplerDesc.maxAnisotropy = 1;
	sampler = device.createSampler(samplerDesc);

	normalMap = AssetManager::LoadTexture2D("assets/images/normalMap.png", wgpu::TextureFormat::RGBA8Unorm);

	Vector<wgpu::BindGroupEntry> bindings(3, wgpu::Default);

	{
		wgpu::BindGroupEntry& binding = bindings[0];
		binding.binding = 0;
		binding.offset = 0;
		binding.buffer = uniformBuffer;
		binding.size = sizeof(MoonRenderUniform);
	}

	{
		wgpu::BindGroupEntry& binding = bindings[1];
		binding.binding = 1;
		binding.sampler = sampler;
	}

	{
		wgpu::BindGroupEntry& binding = bindings[2];
		binding.binding = 2;
		binding.textureView = normalMap->GetTextureView();
	}

	wgpu::BindGroupDescriptor bindGroupDesc{};
	bindGroupDesc.layout = bindGroupLayout;
	bindGroupDesc.entryCount = (uint32_t)bindings.size();
	bindGroupDesc.entries = bindings.data();
	bindGroup = device.createBindGroup(bindGroupDesc);

	wgpu::PipelineLayoutDescriptor layoutDesc;
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = (WGPUBindGroupLayout*)&bindGroupLayout;
	layout = device.createPipelineLayout(layoutDesc);
	pipelineDesc.layout = layout;

	pipeline = device.createRenderPipeline(pipelineDesc);
}

MoonRenderPipeline::~MoonRenderPipeline()
{
	pipeline.release();
	layout.release();
	bindGroup.release();
	uniformBuffer.destroy();
	uniformBuffer.release();
	delete normalMap;
	sampler.release();
	bindGroupLayout.release();
	shaderModule.release();
}

void MoonRenderPipeline::BindToRenderPass(wgpu::RenderPassEncoder renderPass)
{
	renderPass.setPipeline(pipeline);
	renderPass.setBindGroup(0, bindGroup, 0, nullptr);
}
