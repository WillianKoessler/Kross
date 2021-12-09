#include "Kross_pch.h"

#include "Kross/Core/Input.h"

#include "Kross/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Kross {
	glm::vec2 Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(
			static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()),
			&xpos, &ypos
		);
		return { (float)xpos, (float)ypos };
	}
	void Input::SetMousePosition(const glm::vec2& pos)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()),
			pos.x, pos.y
		);
	}
	void Input::SetMousePosition(double x, double y)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()),
			x, y
		);
	}
	float Input::GetMouseX() { return GetMousePosition().x; }
	float Input::GetMouseY() { return GetMousePosition().y; }

	Input::KeyState Input::GetKeyState(Key keycode)
	{
		static constexpr uint16_t maxKeys = 512u;
		static Input::KeyState keys[maxKeys] = { Input::KeyState::NOT_PRESSED };
		static bool s_bOldKeys[maxKeys] = { false };
		static bool s_bKeys[maxKeys] = { false };

		int code = (int)keycode;
		int nState = glfwGetKey(static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()), code);
		Input::KeyState &now = keys[code];
		bool &state = s_bKeys[code];
		bool &oldState = s_bOldKeys[code];

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
	Input::MouseButtonState Input::GetMouseButtonState(MouseButton keycode)
	{
		static constexpr uint16_t maxKeys = 16u;
		static Input::MouseButtonState keys[maxKeys] = { Input::MouseButtonState::NOT_PRESSED };
		static bool s_bOldKeys[maxKeys] = { false };
		static bool s_bKeys[maxKeys] = { false };

		int code = (int)keycode;
		int nState = glfwGetMouseButton(static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow()), code);
		Input::MouseButtonState &now = keys[code];
		bool &state = s_bKeys[code];
		bool &oldState = s_bOldKeys[code];

		switch (nState) {
			case GLFW_PRESS:
				{
					state = true;
					now = Input::MouseButtonState::HELD;
					break;
				}
			case GLFW_RELEASE:
				{
					state = false;
					now = Input::MouseButtonState::NOT_PRESSED;
					break;
				}
		}
		if (state != oldState) {
			if (now == Input::MouseButtonState::HELD)		 now = Input::MouseButtonState::PRESSED;
			if (now == Input::MouseButtonState::NOT_PRESSED) now = Input::MouseButtonState::RELEASED;
		}
		oldState = state;
		return now;
	}
	bool Input::IsKeyHeld(Key keycode) { return GetKeyState(keycode) == Input::KeyState::HELD; }
	bool Input::IsKeyPressed(Key keycode) { return GetKeyState(keycode) == Input::KeyState::PRESSED; }
	bool Input::IsKeyReleased(Key keycode) { return GetKeyState(keycode) == Input::KeyState::RELEASED; }
	bool Input::IsMouseButtonHeld(MouseButton keycode) { return GetMouseButtonState(keycode) == Input::MouseButtonState::HELD; }
	bool Input::IsMouseButtonPressed(MouseButton keycode) { return GetMouseButtonState(keycode) == Input::MouseButtonState::PRESSED; }
	bool Input::IsMouseButtonReleased(MouseButton keycode) { return GetMouseButtonState(keycode) == Input::MouseButtonState::RELEASED; }
}