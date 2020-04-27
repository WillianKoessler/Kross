#include "Kross_pch.h"
#include "WindowInput.h"

#include "Kross/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Kross {
	Input* Input::s_Instance = new WindowInput();
	bool WindowInput::IsKeyPressedImpl(int keycode)
	{
		int state = glfwGetKey(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowInput::IsKeyReleasedImpl(int keycode)
	{
		int state = glfwGetKey(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			keycode);
		if (state == GLFW_PRESS || state == GLFW_REPEAT)
		{
			state = glfwGetKey(
				static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
				keycode);
			return state == GLFW_RELEASE;
		}
		else return false;
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
	const glm::dvec2& WindowInput::GetMousePosImpl()
	{
		glfwGetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			&mouse_position.x,
			&mouse_position.y
		);
		return mouse_position;
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
	void WindowInput::SetMousePosImpl(const glm::dvec2& pos)
	{
		glfwSetCursorPos(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			pos.x,
			pos.y
		);
	}
	void WindowInput::HideCursorImpl()
	{
		glfwSetInputMode(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			GLFW_CURSOR,
			GLFW_CURSOR_DISABLED
		);
	}
	void WindowInput::ShowCursorImpl()
	{
		glfwSetInputMode(
			static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()),
			GLFW_CURSOR,
			GLFW_CURSOR_NORMAL
		);
	}
}