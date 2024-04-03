#pragma once

#include "core/KeyCode.h"
#include "core/MouseCode.h"
#include "util/Math.h"

class Input
{
public:
	static bool PressedKey(Key key);
	static bool PressedMouseButton(MouseButton mouseButton);
	static Vec2 GetMousePosition();
};
