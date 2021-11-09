#pragma once
#include "Kross/Core/Input.h"

namespace Kross{

	class WindowInput : public Input
	{
		static KeyState keys[256];
		static bool s_bOldKeys[256];
		static bool s_bKeys[256];
	protected:
		virtual KeyState GetKeyStateImpl(int keycode) override;
		virtual bool IsKeyHeldImpl(int keycode) override;
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyReleasedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual void SetMousePositionImpl(double x, double y) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}