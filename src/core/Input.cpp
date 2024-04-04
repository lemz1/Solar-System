#include "Input.h"

#include "core/Application.h"

Vec2 Input::_Scroll = Vec2(0.0f, 0.0f);

bool Input::PressedKey(Key key)
{
    GLFWwindow* window = Application::GetWindow()->GetHandle();
    return glfwGetKey(window, (KeyCode)key) == GLFW_PRESS;
}

bool Input::PressedMouseButton(MouseButton mouseButton)
{
    GLFWwindow* window = Application::GetWindow()->GetHandle();
    return glfwGetMouseButton(window, (MouseCode)mouseButton) == GLFW_PRESS;
}

Vec2 Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(Application::GetWindow()->GetHandle(), &x, &y);
	return Vec2(x, y);
}
