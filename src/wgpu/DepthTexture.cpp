#include "DepthTexture.h"

#include "core/Application.h"

wgpu::TextureFormat DepthTexture::_Format = wgpu::TextureFormat::Depth24Plus;

DepthTexture::DepthTexture(
	uint32_t width,
	uint32_t height
)
:	_width(width),
	_height(height),
	_texture(nullptr),
	_textureView(nullptr)
{
	CreateTexture();
}

DepthTexture::~DepthTexture()
{
	_textureView.release();
	_texture.destroy();
	_texture.release();
}

void DepthTexture::Resize(
	uint32_t width, 
	uint32_t height
)
{
	if (_width == width && _height == height) 
	{
		return;
	}

	_width = width;
	_height = height;

	_textureView.release();
	_texture.destroy();
	_texture.release();

	CreateTexture();
}

void DepthTexture::CreateTexture()
{
	wgpu::Device device = Application::GetWGPUContext()->device;

	wgpu::TextureDescriptor textureDesc = wgpu::Default;
	textureDesc.dimension = wgpu::TextureDimension::_2D;
	textureDesc.format = DepthTexture::_Format;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.size = { _width, _height, 1 };
	textureDesc.usage = wgpu::TextureUsage::RenderAttachment;
	textureDesc.viewFormatCount = 1;
	textureDesc.viewFormats = (WGPUTextureFormat*)&DepthTexture::_Format;
	textureDesc.label = "Depth Texture";
	_texture = device.createTexture(textureDesc);

	wgpu::TextureViewDescriptor textureViewDesc = wgpu::Default;
	textureViewDesc.aspect = wgpu::TextureAspect::DepthOnly;
	textureViewDesc.baseArrayLayer = 0;
	textureViewDesc.arrayLayerCount = 1;
	textureViewDesc.baseMipLevel = 0;
	textureViewDesc.mipLevelCount = 1;
	textureViewDesc.dimension = wgpu::TextureViewDimension::_2D;
	textureViewDesc.format = DepthTexture::_Format;
	textureViewDesc.label = "Depth Texture View";
	_textureView = _texture.createView(textureViewDesc);
}
