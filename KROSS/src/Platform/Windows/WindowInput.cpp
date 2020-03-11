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
	float WindowInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}
	float WindowInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}
}