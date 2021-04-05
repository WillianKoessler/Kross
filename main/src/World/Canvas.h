#pragma once
#include "Kross.h"
#include "World/Creatures.h"

class Canvas final : public Kross::Layer
{
#define MGR_RUN_APPLICATION		0x01
#define MGR_RENDERER_STATS		0x02
#define MGR_STYLE_EDITOR		0x04
#define MGR_SHOW_CAMERA			0x08
#define MGR_DEMO_WINDOW			0x10
#define MGR_POPUP_MSG			0x20
#define MGR_FULLSCREEN			0x40
#define MGR_FULLSCREEN_INTER	0x80

	/*
	* RUN_APPLICATION	-> keeps application running
	* RENDERER_STATS	-> whether to display the renderer status (batches, drawcalls, etc)
	* STYLE_EDITOR		-> whether to display ImGui Style Editor
	* SHOW_CAMERA		-> whether to display Primary Camera Settings
	* DEMO_WINDOW		-> whether to display ImGui Demo Window
	* POPUP_MSG			-> whether to display a popup message
	* FULLSCREEN		-> whether FullScreen feature is active or inactive
	* FULLSCREEN_INTER	-> internal use only
	*/
	//unsigned char manager = MGR_RUN_APPLICATION | MGR_SHOW_CAMERA | MGR_FULLSCREEN_INTER;

	// main camera
	Kross::Ref<Kross::Camera::Controller> camera;

	// size for test grid
	float size = 10;
	
	// vector of entities (now creatures. fix later)
	std::vector<Creature> entities;

	// acceleration vector for main entity
	glm::vec3 acc = { 0.0f, 0.0f, 0.0f };
	
	// parameter for batch rendering quads
	Kross::QuadParams params;

	// texture atlas
	Kross::Ref<Kross::Texture::T2DAtlas> atlas;

	int MainWindow(bool* p_open, const char* p_id);
	void Status(bool* show_rendererStats, float ts);

public:
	Canvas();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Kross::Timestep) override;
	virtual void OnImGuiRender(Kross::Timestep) override;
	virtual void OnEvent(Kross::Event& event) override;
};

