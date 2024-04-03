#pragma once

#include <webgpu/webgpu.hpp>

class Texture2D
{
public:
	// only RGBA8Unorm and RGBA32Float supported
	Texture2D(
		void* pixels,
		uint32_t width,
		uint32_t height,
		wgpu::TextureFormat textureFormat
	);
	~Texture2D();

	void Write(void* pixels);

	uint32_t GetWidth() const { return _width; }
	uint32_t GetHeight() const { return _height; }
	wgpu::Texture GetTexture() const { return _texture; }
	wgpu::TextureView GetTextureView() const { return _textureView; }
	wgpu::TextureFormat GetTextureFormat() const { return _textureFormat; }
private:
	uint32_t _width;
	uint32_t _height;

	wgpu::Texture _texture;
	wgpu::TextureView _textureView;
	wgpu::TextureFormat _textureFormat;
};
