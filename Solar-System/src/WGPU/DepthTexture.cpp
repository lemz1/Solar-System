#include "DepthTexture.h"

#include "Core/Application.h"

using namespace wgpu;

TextureFormat DepthTexture::_Format = TextureFormat::Depth24Plus;

DepthTexture::DepthTexture(
	uint32_t width,
	uint32_t height
)
:	_depthTexture(nullptr),
	_depthTextureView(nullptr)
{
	Device device = Application::GetWGPUContext()->device;

	TextureDescriptor depthTextureDesc;
	depthTextureDesc.dimension = TextureDimension::_2D;
	depthTextureDesc.format = DepthTexture::_Format;
	depthTextureDesc.mipLevelCount = 1;
	depthTextureDesc.sampleCount = 1;
	depthTextureDesc.size = { width, height, 1 };
	depthTextureDesc.usage = TextureUsage::RenderAttachment;
	depthTextureDesc.viewFormatCount = 1;
	depthTextureDesc.viewFormats = (WGPUTextureFormat*)&DepthTexture::_Format;
	_depthTexture = device.createTexture(depthTextureDesc);

	TextureViewDescriptor depthTextureViewDesc;
	depthTextureViewDesc.aspect = TextureAspect::DepthOnly;
	depthTextureViewDesc.baseArrayLayer = 0;
	depthTextureViewDesc.arrayLayerCount = 1;
	depthTextureViewDesc.baseMipLevel = 0;
	depthTextureViewDesc.mipLevelCount = 1;
	depthTextureViewDesc.dimension = TextureViewDimension::_2D;
	depthTextureViewDesc.format = DepthTexture::_Format;
	_depthTextureView = _depthTexture.createView(depthTextureViewDesc);
}

DepthTexture::~DepthTexture()
{
	_depthTextureView.release();
	_depthTexture.destroy();
	_depthTexture.release();
}
