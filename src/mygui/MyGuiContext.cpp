#include "MyGuiContext.h"

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_wgpu.h>

#include "core/Application.h"
#include "wgpu/DepthTexture.h"

MyGuiContext::MyGuiContext()
{
    wgpu::Device device = Application::GetWGPUContext()->device;
    wgpu::TextureFormat swapChainFormat = Application::GetWGPUContext()->swapChainFormat;
    wgpu::TextureFormat depthTextureFormat = DepthTexture::GetTextureFormat();
    Window* window = Application::GetWindow();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOther(window->GetHandle(), true);

    ImGui_ImplWGPU_InitInfo info = {};
    info.Device = device;
    info.NumFramesInFlight = 3;
    info.RenderTargetFormat = swapChainFormat;
    info.DepthStencilFormat = depthTextureFormat;
    ImGui_ImplWGPU_Init(&info);
}

MyGuiContext::~MyGuiContext()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplWGPU_Shutdown();
}

void MyGuiContext::StartFrame()
{
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void MyGuiContext::EndFrame()
{
    wgpu::RenderPassEncoder renderPass = Application::GetWGPUContext()->renderPass;

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), renderPass);
}
