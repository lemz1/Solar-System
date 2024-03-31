#pragma once

#include "Core/KeyCode.h"
#include "Core/MouseCode.h"
#include "Util/Math.h"

class Input
{
public:
	static bool PressedKey(Key key);
	static bool PressedMouseButton(MouseButton mouseButton);
	static Vec2 GetMousePosition();
};
