#pragma once

#include "util/Camera.h"

class CameraController
{
public:
	CameraController(Camera* camera);

	void OnUpdate(float deltaTime);

	const Camera* GetCamera() const { return _camera; }

	void SetDistance(float distance) { _distance = distance; RotateCameraAroundOrigin(); }
	float GetDistance() const { return _distance; }
private:
	void RotateCameraAroundOrigin();
private:
	Camera* _camera;

	Vec2 _lastMousePos{ 0.0f };
	float _pitch = 0;
	float _yaw = 0;
	float _distance = 3.0f;

	// for animation
	Vec2 _drag{ 0.0f };
};
