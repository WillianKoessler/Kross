#pragma once
#include "Kross.h"
#include "World/Creatures.h"

constexpr char MGR_RUN_APPLICATION	= (1<<0);	// keeps application running
constexpr char MGR_RENDERER_STATS	= (1<<1);	// whether to display the renderer status (batches, drawcalls, etc)
constexpr char MGR_STYLE_EDITOR		= (1<<2);	// whether to display ImGui Style Editor
constexpr char MGR_SHOW_CAMERA		= (1<<3);	// whether to display Primary Camera Settings
constexpr char MGR_DEMO_WINDOW		= (1<<4);	// whether to display ImGui Demo Window
constexpr char MGR_POPUP_MSG		= (1<<5);	// whether to display a popup message
constexpr char MGR_FULLSCREEN		= (1<<6);	// whether FullScreen feature is active or inactive
constexpr char MGR_FULLSCREEN_INTER	= (1<<7);	// internal use only

/*
	bool opt_fullscreen_persistant = true;
	bool fullscreen = false;
	bool run_applicacion = true;
	bool show_rendererStats = false;
	bool show_camera = true;
	bool show_demo_window = false;
	bool show_style_editor = false;
	bool popup_message_box = false;
	bool viewport = true;
	bool keyboardEnabled = true; bool keyboardState = true;
	bool gamepadEnabled = true; bool gamepadState = true;
	bool viewportsEnabled = false; bool viewportsState = false;
*/
class UI final : public Kross::Layer
{
	enum UI_ {
		RUN_APPLICATION	 = (1<<0),
		RENDERER_STATS	 = (1<<1),
		STYLE_EDITOR	 = (1<<2),
		SHOW_CAMERA		 = (1<<3),
		DEMO_WINDOW		 = (1<<4),
		POPUP_MSG		 = (1<<5),
		FULLSCREEN		 = (1<<6),
		FULLSCREEN_INTER = (1<<7)
	};
	UI_ flags = RUN_APPLICATION;
	int MainWindow(bool* p_open, const char* p_id);
	void viewport(bool* p_open);
	void Status(bool* show_rendererStats, float ts);
	Kross::Ref<Kross::Texture::T2D> canvas;
public:
	UI();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Kross::Timestep) override;
	virtual void OnImGuiRender(Kross::Timestep) override;
	virtual void OnEvent(Kross::Event& event) override;
	void setCanvas(const Kross::Ref<Kross::Texture::T2D>& texture);
};

