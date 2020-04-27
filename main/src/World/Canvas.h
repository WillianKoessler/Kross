#pragma once
#include "Kross.h"

class Canvas final : public Kross::Layer
{
public:
	Canvas();

	virtual void OnAttach() override;
	virtual void OnUpdate(Kross::Timestep) override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender(Kross::Timestep) override;

private:
	Kross::Ref<Kross::Camera::Controller> camera;
	glm::vec3 p = { 0.0f, 0.0f, 2.0f };

	// Current Renderer
	int current = 0;

	Kross::QuadParams params;

private:

	// Main Screen (DockSpace)
	uint32_t MainScreen(bool* p_open, const char* ID);

	// Status Window
	void Status(bool* show_rendererStats, float ts);

	// Saves ImGui Style on a file (imguiStyle.ini by default)
	void SaveStyle(ImGuiStyle* style, const std::string& path = "imguiStyle.ini");

	// Load ImGui Style from ini file (imguiStyle.ini by default)
	ImGuiStyle LoadStyle(const std::string& path);

	// Stops EVERYTHING and shows a pre-defined window based on passed ID
	void MessageBoxDialog(bool* show, const char* id, void* data = nullptr);

	// Helper to display a little (?) mark which shows a tooltip when hovered.
	void ShowHelpMarker(const char* desc);
	
	// Toggle a single bit
	inline bool ToggleFlag(bool enable, int& flags, int mask)
	{
		if (enable)
			flags |= mask;
		else
			flags &= ~(flags & mask);
		return enable;
	}
};

