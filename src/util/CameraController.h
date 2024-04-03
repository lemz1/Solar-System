#pragma once

#include "util/Camera.h"

class CameraController
{
public:
	CameraController(Camera* camera);

	void OnUpdate(float deltaTime);

	void SetActive(bool isActive);
	bool IsActive() const { return _isActive; }

	const Camera* GetCamera() const { return _camera; }
private:
	Camera* _camera;

	bool _isActive = true;

	Vec2 _lastMousePos{ 0.0f };
	float _pitch = 0;
	float _yaw = 0;
};
