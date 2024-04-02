#include "AssetManager.h"

#include "Core/Application.h"

#include <iostream>
#include <fstream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

using namespace wgpu;

std::string AssetManager::GetFileContent(const char* filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Couldn't find file: " << filePath << std::endl;
        #ifdef DEBUG
        assert(false);
        #endif
        return "";
    }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();

    return content;
}

wgpu::ShaderModule AssetManager::LoadShaderModule(const char* filePath)
{
    wgpu::Device device = Application::GetWGPUContext()->device;
    std::string shaderSource = AssetManager::GetFileContent(filePath);

    wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc;
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
    shaderCodeDesc.code = shaderSource.c_str();

    wgpu::ShaderModuleDescriptor shaderDesc;
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
    shaderDesc.hintCount = 0;
    shaderDesc.hints = nullptr;
    
    return device.createShaderModule(shaderDesc);
}

Texture2D* AssetManager::GetTexture2D(
    const char* filePath,
    TextureFormat textureFormat
)
{
    void* pixels;
    int width;
    int height;
    int numComponents;
    switch (textureFormat)
    {
        case TextureFormat::RGBA8Unorm:
            pixels = stbi_load(filePath, &width, &height, &numComponents, 4);
            break;
        case TextureFormat::RGBA32Float:
            pixels = stbi_loadf(filePath, &width, &height, &numComponents, 4);
            break;
        default:
            std::cerr << "texture format not supported: " << textureFormat << std::endl;
            return nullptr;
    }

    return new Texture2D(pixels, width, height, textureFormat);
}
