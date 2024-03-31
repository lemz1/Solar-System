#pragma once

#include "Util/Camera.h"

class CameraController
{
public:
	CameraController(Camera* camera);

	void OnUpdate(float deltaTime);

	const Camera* GetCamera() const { return _camera; }
private:
	Camera* _camera;

	Vec2 _lastMousePos;
	float _pitch = 0;
	float _yaw = 0;
};
