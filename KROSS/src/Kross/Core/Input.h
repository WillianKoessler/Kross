#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Events/KeyCodes.h"
#include "Kross/Events/MouseButtonCodes.h"
#include <glm/glm.hpp>

namespace Kross {

	class KAPI Input
	{
	public:
		static enum class KeyState { NOT_PRESSED = 0, PRESSED = 1, HELD = 2, RELEASED = 3 } GetKeyState(Key keycode);
		static enum class MouseButtonState { NOT_PRESSED = 0, PRESSED = 1, HELD = 2, RELEASED = 3 } GetMouseButtonState(Mouse button);
		static bool IsKeyHeld(Key keycode);
		static bool IsKeyPressed(Key keycode);
		static bool IsKeyReleased(Key keycode);
		static bool IsMouseButtonHeld(Mouse button);
		static bool IsMouseButtonPressed(Mouse button);
		static bool IsMouseButtonReleased(Mouse button);
		static glm::vec2 GetMousePosition();
		static void SetMousePosition(const glm::vec2& pos);
		static void SetMousePosition(double x, double y);
		static float GetMouseX();
		static float GetMouseY();
	};
}
