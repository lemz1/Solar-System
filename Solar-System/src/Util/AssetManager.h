#pragma once

#include <string>
#include <webgpu/webgpu.hpp>

class AssetManager
{
public:
	static std::string GetFileContent(const char* filePath);
	static wgpu::ShaderModule LoadShaderModule(const char* filePath);
};
