#pragma once

#include <memory>

#include "Util/Camera.h"

class CameraController
{
public:
	CameraController(Camera* camera);

	void OnUpdate(float deltaTime);

	const Camera* GetCamera() const { return _camera; }
private:
	Camera* _camera;
};
