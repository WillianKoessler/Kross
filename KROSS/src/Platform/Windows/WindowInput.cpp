#include "Kross_pch.h"

#include "Kross/Core/Input.h"

#include "Kross/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Kross {
	constexpr uint16_t maxKeys = 512u;
	Input::KeyState keys[maxKeys] = { Input::KeyState::NOT_PRESSED };
	bool s_bOldKeys[maxKeys] = { false };
	bool s_bKeys[maxKeys] = { false };
	Input::KeyState Kross::Input::GetKeyState(Code keycode)
	{
		int code = (int)keycode;
		int nState = glfwGetKey(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), code);
		Input::KeyState& now = keys[code];
		bool& state = s_bKeys[code];
		bool& oldState = s_bOldKeys[code];

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
	bool Input::IsKeyHeld(Code keycode)
	{
		return GetKeyState(keycode) == Input::KeyState::HELD;
	}
	bool Input::IsKeyPressed(Code keycode)
	{
		return GetKeyState(keycode) == Input::KeyState::PRESSED;
	}
	bool Input::IsKeyReleased(Code keycode)
	{
		return GetKeyState(keycode) == Input::KeyState::RELEASED;
	}
	bool  Input::IsMouseButtonPressed(Code button)
	{
		return glfwGetMouseButton(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			(int)button) == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			&xpos,
			&ypos
		);
		return { (float)xpos, (float)ypos };
	}
	void Input::SetMousePosition(double x, double y)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			x, y
		);
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}