#pragma once

#include <string>
#include <webgpu/webgpu.hpp>
#include "WGPU/Texture2D.h"

class AssetManager
{
public:
	static std::string GetFileContent(const char* filePath);
	static wgpu::ShaderModule LoadShaderModule(const char* filePath);
	static Texture2D* LoadTexture2D(
		const char* filePath,
		wgpu::TextureFormat textureFormat
	);
};
