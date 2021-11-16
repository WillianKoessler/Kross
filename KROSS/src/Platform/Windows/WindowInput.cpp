#include "Kross_pch.h"
#include "WindowInput.h"

#include "Kross/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Kross {
	Input* Input::s_Instance = new WindowInput();
	Input::KeyState WindowInput::keys[maxKeys] = { Input::KeyState::NOT_PRESSED };
	bool WindowInput::s_bOldKeys[maxKeys] = { false };
	bool WindowInput::s_bKeys[maxKeys] = { false };
	Input::KeyState WindowInput::GetKeyStateImpl(int keycode)
	{
		int nState = glfwGetKey(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), keycode);
		Input::KeyState& now = keys[keycode];
		bool& state = s_bKeys[keycode];
		bool& oldState = s_bOldKeys[keycode];

		switch (nState) {
		case GLFW_PRESS:
		{
			state = true;
			now = Input::KeyState::HELD;
			break;
		}
		case GLFW_RELEASE:
		{
			state = false;
			now = Input::KeyState::NOT_PRESSED;
			break;
		}
		}
		if (state != oldState) {
			if (now == Input::KeyState::HELD) now = Input::KeyState::PRESSED;
			if (now == Input::KeyState::NOT_PRESSED) now = Input::KeyState::RELEASED;
		}
		oldState = state;
		return now;
	}
	bool WindowInput::IsKeyHeldImpl(int keycode)
	{
		return GetKeyStateImpl(keycode) == Input::KeyState::HELD;
	}
	bool WindowInput::IsKeyPressedImpl(int keycode)
	{
		return GetKeyStateImpl(keycode) == Input::KeyState::PRESSED;
	}
	bool WindowInput::IsKeyReleasedImpl(int keycode)
	{
		return GetKeyStateImpl(keycode) == Input::KeyState::RELEASED;
	}
	bool  WindowInput::IsMouseButtonPressedImpl(int button)
	{
		return glfwGetMouseButton(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			button) == GLFW_PRESS;
	}
	std::pair<float, float> WindowInput::GetMousePositionImpl()
	{
		double xpos, ypos;
		glfwGetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			&xpos,
			&ypos
		);
		return { (float)xpos, (float)ypos };
	}
	void WindowInput::SetMousePositionImpl(double x, double y)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			x, y
		);
	}
	float WindowInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}
	float WindowInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}