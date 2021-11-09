#pragma once

#include "Kross/Core/Core.h"

namespace Kross {

	class KAPI Input
	{
	public:
		enum class KeyState {
			NOT_PRESSED = 0,
			PRESSED,
			HELD,
			RELEASED
		};

		inline static KeyState GetKeyState(int keycode) { return s_Instance->GetKeyStateImpl(keycode); }
		inline static bool IsKeyHeld(int keycode) { return s_Instance->IsKeyHeldImpl(keycode); }
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsKeyReleased(int keycode) { return s_Instance->IsKeyReleasedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static void SetMousePosition(double x, double y) { s_Instance->SetMousePositionImpl(x, y); }
		inline static float IsGetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float IsGetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual KeyState GetKeyStateImpl(int keycode) = 0;
		virtual bool IsKeyHeldImpl(int keycode) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyReleasedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual void SetMousePositionImpl(double x, double y) = 0;
	private:
		static Input* s_Instance;
	};
}
