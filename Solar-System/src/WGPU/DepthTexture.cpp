#include "DepthTexture.h"

#include "Core/Application.h"

using namespace wgpu;

TextureFormat DepthTexture::_Format = TextureFormat::Depth24Plus;

DepthTexture::DepthTexture(
	uint32_t width,
	uint32_t height
)
:	_width(width),
	_height(height),
	_texture(nullptr),
	_textureView(nullptr)
{
	Device device = Application::GetWGPUContext()->device;

	TextureDescriptor textureDesc;
	textureDesc.dimension = TextureDimension::_2D;
	textureDesc.format = DepthTexture::_Format;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.size = { width, height, 1 };
	textureDesc.usage = TextureUsage::RenderAttachment;
	textureDesc.viewFormatCount = 1;
	textureDesc.viewFormats = (WGPUTextureFormat*)&DepthTexture::_Format;
	textureDesc.label = "Depth Texture";
	_texture = device.createTexture(textureDesc);

	TextureViewDescriptor textureViewDesc;
	textureViewDesc.aspect = TextureAspect::DepthOnly;
	textureViewDesc.baseArrayLayer = 0;
	textureViewDesc.arrayLayerCount = 1;
	textureViewDesc.baseMipLevel = 0;
	textureViewDesc.mipLevelCount = 1;
	textureViewDesc.dimension = TextureViewDimension::_2D;
	textureViewDesc.format = DepthTexture::_Format;
	textureViewDesc.label = "Depth Texture View";
	_textureView = _texture.createView(textureViewDesc);
}

DepthTexture::~DepthTexture()
{
	_textureView.release();
	_texture.destroy();
	_texture.release();
}
