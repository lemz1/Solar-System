#include "Texture2D.h"

#include "core/Application.h"

Texture2D::Texture2D(
	void* pixels,
	uint32_t width, 
	uint32_t height,
	wgpu::TextureFormat textureFormat
)
:	_width(width),
	_height(height),
	_texture(nullptr),
	_textureView(nullptr),
	_textureFormat(textureFormat)
{
	wgpu::Device device = Application::GetWGPUContext()->device;

	wgpu::TextureDescriptor textureDesc;
	textureDesc.dimension = wgpu::TextureDimension::_2D;
	textureDesc.format = _textureFormat;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.size = { width, height, 1 };
	textureDesc.usage = wgpu::TextureUsage::CopyDst | wgpu::TextureUsage::TextureBinding;
	textureDesc.viewFormatCount = 0;
	textureDesc.viewFormats = nullptr;
	textureDesc.label = "Texture";
	_texture = device.createTexture(textureDesc);

	wgpu::TextureViewDescriptor textureViewDesc;
	textureViewDesc.aspect = wgpu::TextureAspect::All;
	textureViewDesc.baseArrayLayer = 0;
	textureViewDesc.arrayLayerCount = 1;
	textureViewDesc.baseMipLevel = 0;
	textureViewDesc.mipLevelCount = 1;
	textureViewDesc.dimension = wgpu::TextureViewDimension::_2D;
	textureViewDesc.format = _textureFormat;
	textureViewDesc.label = "Depth Texture View";
	_textureView = _texture.createView(textureViewDesc);

	Write(pixels);
}

Texture2D::~Texture2D()
{
	_textureView.release();
	_texture.destroy();
	_texture.release();
}

void Texture2D::Write(void* pixels)
{
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	uint8_t bytesPerPixel;
	switch (_textureFormat)
	{
		case wgpu::TextureFormat::RGBA8Unorm:
			bytesPerPixel = 4;
			break;
		case wgpu::TextureFormat::RGBA32Float:
			bytesPerPixel = 16;
			break;
		default:
			std::cerr << "could not write to texture because texture format not supported" << _textureFormat << std::endl;
			return;
	}
	wgpu::ImageCopyTexture destination;
	destination.texture = _texture;
	destination.mipLevel = 0;
	destination.origin = { 0, 0, 0 };
	destination.aspect = wgpu::TextureAspect::All;

	wgpu::TextureDataLayout source;
	source.offset = 0;
	source.bytesPerRow = bytesPerPixel * _width;
	source.rowsPerImage = _height;

	queue.writeTexture(destination, pixels, bytesPerPixel * _width * _height, source, {_width, _height, 1});
}
