#include "CameraController.h"

#include "core/Input.h"
#include "core/Application.h"

CameraController::CameraController(Camera* camera)
:	_camera(camera)
{
	_camera->SetPosition(Vec3(0.0f, 0.0f, -3.0f));
}

void CameraController::OnUpdate(float deltaTime)
{
	glm::vec2 mousePos = Input::GetMousePosition();
	glm::vec2 deltaMousePos = mousePos - _lastMousePos;
	_lastMousePos = mousePos;

	// doing this check because on the first frame
	// there are weird shenanigans
	static bool firstTime = true;
	if (firstTime)
	{
		firstTime = false;
		deltaMousePos = Vec2(0.0f);
	}

	// zoom
	Vec2 scroll = Input::GetScroll();
	float distance = _distance - scroll.y * 0.5f;
	distance = Math::Clamp(distance, 1.1f, 5.0f);
	SetDistance(distance);

	// rotation
	Quat cameraRotation = _camera->GetRotation();

	Vec3 upDirection = glm::rotate(cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));

	constexpr float sensitivity = 0.001f;
	if (Input::PressedMouseButton(MouseButton::ButtonLeft) && Input::PressedKey(Key::LeftShift))
	{
		// stopping a bit faster
		_drag.x = Math::Lerp(0.0f, _drag.x, powf(0.5f, deltaTime * 30.0f));
		_drag.y = Math::Lerp(0.0f, _drag.y, powf(0.5f, deltaTime * 30.0f));

		_drag += deltaMousePos * sensitivity;
	}
	else 
	{
		_drag.x = Math::Lerp(0.0f, _drag.x, powf(0.5f, deltaTime * 20.0f));
		_drag.y = Math::Lerp(0.0f, _drag.y, powf(0.5f, deltaTime * 20.0f));
	} 

	_pitch += _drag.y;
	// Sign is used to make it so that when we are upside down
	// the horizontal movement isnt inverted
	_yaw += Math::Sign(upDirection.y) * _drag.x;

	cameraRotation = glm::quat(glm::vec3(_pitch, _yaw, 0.0f));

	_camera->SetRotation(cameraRotation);
	RotateCameraAroundOrigin();
}

void CameraController::RotateCameraAroundOrigin()
{
	Vec3 backwardDirection = glm::rotate(_camera->GetRotation(), glm::vec3(0.0f, 0.0f, -1.0f));

	_camera->SetPosition(backwardDirection * _distance);
}
