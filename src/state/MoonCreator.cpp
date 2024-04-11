#include "MoonCreator.h"

#include "core/Application.h"
#include "util/AssetManager.h"
#include "core/Input.h"
#include "celestial/MoonComputePipeline.h"
#include "debug/Timer.h"

#include <functional>

void MoonCreator::OnCreate()
{
	wgpu::Device device = Application::GetWGPUContext()->device;
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	_moon = new Moon();

	_camera = new Camera(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

	_cameraController = new CameraController(_camera);
}

void MoonCreator::OnEvent(const Event& event)
{
	Event::Dispatch<WindowResizeEvent>(event, BindEventFunction(MoonCreator::OnResize));
}

void MoonCreator::OnUpdate(float deltaTime)
{
	wgpu::Queue queue = Application::GetWGPUContext()->queue;

	_cameraController->OnUpdate(deltaTime);

	float time = Application::GetTime();
	float frequency = 1.0f + time * 3.0f;
	queue.writeBuffer(_moon->GetComputeUniformBuffer()->GetBuffer(), offsetof(MoonComputeUniform, frequency), &frequency, sizeof(float));

	_moon->Generate(4);

	Mat4 viewProjection = _camera->GetProjection() * _camera->GetInverseView();

	queue.writeBuffer(_moon->GetRenderPipeline()->uniformBuffer, offsetof(MoonRenderUniform, viewProjection), &viewProjection, sizeof(Mat4));
}

void MoonCreator::OnDraw(wgpu::RenderPassEncoder renderPass)
{
	_moon->Draw(renderPass);
}

void MoonCreator::OnDrawImGui()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Test");
	ImGui::Text("Test");
	ImGui::End();
}

void MoonCreator::OnDestroy()
{
	delete _cameraController;
	delete _camera;
	delete _moon;
}

void MoonCreator::OnResize(const WindowResizeEvent &event)
{
	_camera->Resize(event.GetWidth(), event.GetHeight());
}
