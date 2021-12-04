#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Events/KeyCodes.h"
#include "Kross/Events/MouseButtonCodes.h"

namespace Kross {

	class KAPI Input
	{
	public:
		static enum class KeyState { NOT_PRESSED = 0, PRESSED = 1, HELD = 2, RELEASED = 3 } GetKeyState(Code keycode);
		static bool IsKeyHeld(Code keycode);
		static bool IsKeyPressed(Code keycode);
		static bool IsKeyReleased(Code keycode);
		static bool IsMouseButtonPressed(Code button);
		static std::pair<float, float> GetMousePosition();
		static void SetMousePosition(double x, double y);
		static float GetMouseX();
		static float GetMouseY();
	};
}
