#include "Camera.h"

Camera::Camera(
	uint32_t width,
	uint32_t height
)
:	_view(1.0f),
	_projection(1.0f),
	_width(width),
	_height(height),
	_verticalFOV(45.0f),
	_nearClipPlane(0.1f),
	_farClipPlane(100.0f)
{
	RecalculateView();
	RecalculateProjection();
}

Camera::~Camera()
{

}

void Camera::RecalculateProjection()
{
	_projection = glm::perspectiveFov(
		glm::radians(_verticalFOV), 
		(float)_width, 
		(float)_height, 
		_nearClipPlane, 
		_farClipPlane
	);
	_inverseProjection = glm::inverse(_projection);
}

void Camera::RecalculateView()
{
	_view = glm::translate(glm::mat4(1.f), _position) * glm::mat4_cast(_rotation);
	_inverseView = glm::inverse(_view);
}

void Camera::Resize(
	uint32_t width, 
	uint32_t height
)
{
	if (_width == width && _height == height)
	{
		return;
	}

	_width = width;
	_height = height;

	RecalculateProjection();
}
