#include "Texture2D.h"

#include "Core/Application.h"

using namespace wgpu;

Texture2D::Texture2D(
	void* pixels,
	uint32_t width, 
	uint32_t height,
	TextureFormat textureFormat
)
:	_width(width),
	_height(height),
	_texture(nullptr),
	_textureView(nullptr),
	_textureFormat(textureFormat)
{
	Device device = Application::GetWGPUContext()->device;

	TextureDescriptor textureDesc;
	textureDesc.dimension = TextureDimension::_2D;
	textureDesc.format = _textureFormat;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.size = { width, height, 1 };
	textureDesc.usage = TextureUsage::CopyDst | TextureUsage::TextureBinding;
	textureDesc.viewFormatCount = 0;
	textureDesc.viewFormats = nullptr;
	textureDesc.label = "Texture";
	_texture = device.createTexture(textureDesc);

	TextureViewDescriptor textureViewDesc;
	textureViewDesc.aspect = TextureAspect::All;
	textureViewDesc.baseArrayLayer = 0;
	textureViewDesc.arrayLayerCount = 1;
	textureViewDesc.baseMipLevel = 0;
	textureViewDesc.mipLevelCount = 1;
	textureViewDesc.dimension = TextureViewDimension::_2D;
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
	Queue queue = Application::GetWGPUContext()->queue;

	ImageCopyTexture destination;
	destination.texture = _texture;
	destination.mipLevel = 0;
	destination.origin = { 0, 0, 0 };
	destination.aspect = TextureAspect::All;

	TextureDataLayout source;
	source.offset = 0;
	switch (_textureFormat)
	{
		case TextureFormat::RGBA8Unorm:
			source.bytesPerRow = 4 * _width;
			break;
		case TextureFormat::RGBA32Float:
			source.bytesPerRow = 16 * _width;
			break;
	}
	source.rowsPerImage = _height;

	queue.writeTexture(destination, pixels, _width * _height, source, {_width, _height, 1});
}
