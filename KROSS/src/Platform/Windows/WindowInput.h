#pragma once
#include "Kross/Core/Input.h"

namespace Kross{
	class WindowInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyReleasedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual const glm::dvec2& GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

		virtual void SetMousePosImpl(const glm::dvec2& pos) override;

		virtual void HideCursorImpl() override;
		virtual void ShowCursorImpl() override;
	};
}