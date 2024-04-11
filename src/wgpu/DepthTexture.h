#pragma once

#include <webgpu/webgpu.hpp>

class DepthTexture
{
public:
	DepthTexture(
		wgpu::Device device,
		uint32_t width,
		uint32_t height
	);
	DepthTexture(
		uint32_t width,
		uint32_t height
	);
	~DepthTexture();

	void Resize(
		uint32_t width,
		uint32_t height
	);

	uint32_t GetWidth() const { return _width; }
	uint32_t GetHeight() const { return _height; }
	wgpu::Texture GetTexture() const { return _texture; }
	wgpu::TextureView GetTextureView() const { return _textureView; }

	static wgpu::TextureFormat GetTextureFormat() { return DepthTexture::_Format; }
private:
	void CreateTexture(wgpu::Device device);
	void CreateTexture();
private:
	static wgpu::TextureFormat _Format;

	uint32_t _width;
	uint32_t _height;

	wgpu::Texture _texture;
	wgpu::TextureView _textureView;
};
