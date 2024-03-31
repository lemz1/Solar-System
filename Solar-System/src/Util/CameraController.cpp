#include "CameraController.h"

#include "Core/Input.h"
#include "Core/Application.h"

CameraController::CameraController(Camera* camera)
:	_camera(camera),
	_lastMousePos(Input::GetMousePosition())
{}

void CameraController::OnUpdate(float deltaTime)
{
	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 deltaMousePos = mousePos - _lastMousePos;
	_lastMousePos = mousePos;

	GLFWwindow* window = Application::GetWindow()->GetHandle();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Vec3 cameraPosition = _camera->GetPosition();
	Quat cameraRotation = _camera->GetRotation();

	Vec3 rightDirection = glm::rotate(cameraRotation, glm::vec3(1.f, 0.f, 0.f));
	Vec3 upDirection = glm::rotate(cameraRotation, glm::vec3(0.f, 1.f, 0.f));
	Vec3 forwardDirection = glm::rotate(cameraRotation, glm::vec3(0.f, 0.f, 1.f));

	if (Input::PressedKey(Key::D))
	{
		cameraPosition += rightDirection * 5.0f * deltaTime;
	}
	else if (Input::PressedKey(Key::A))
	{
		cameraPosition -= rightDirection * 5.0f * deltaTime;
	}

	if (Input::PressedKey(Key::E))
	{
		cameraPosition += upDirection * 5.0f * deltaTime;
	}
	else if (Input::PressedKey(Key::Q))
	{
		cameraPosition -= upDirection * 5.0f * deltaTime;
	}

	if (Input::PressedKey(Key::W))
	{
		cameraPosition += forwardDirection * 5.0f * deltaTime;
	}
	else if (Input::PressedKey(Key::S))
	{
		cameraPosition -= forwardDirection * 5.0f * deltaTime;
	}

	_pitch += deltaMousePos.y * 0.001f;
	_yaw += Math::Sign(upDirection.y) * deltaMousePos.x * 0.001f;

	cameraRotation = glm::quat(glm::vec3(_pitch, _yaw, 0.f));

	_camera->SetPosition(cameraPosition);
	_camera->SetRotation(cameraRotation);
}
