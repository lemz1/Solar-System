#pragma once

#include <webgpu/webgpu.hpp>

class DepthTexture
{
public:
	DepthTexture(
		uint32_t width,
		uint32_t height
	);
	~DepthTexture();

	wgpu::Texture GetTexture() const { return _depthTexture; }
	wgpu::TextureView GetTextureView() const { return _depthTextureView; }

	static wgpu::TextureFormat GetFormat() { return DepthTexture::_Format; }
private:
	static wgpu::TextureFormat _Format;

	wgpu::Texture _depthTexture;
	wgpu::TextureView _depthTextureView;
};
