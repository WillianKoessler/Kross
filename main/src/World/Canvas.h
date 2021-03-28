#pragma once
#include "Kross.h"
#include "World/Creatures.h"

class Canvas final : public Kross::Layer
{
public:
	Canvas();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Kross::Timestep) override;
	virtual void OnImGuiRender(Kross::Timestep) override;
	virtual void OnEvent(Kross::Event& event) override;

private:
	// main camera
	Kross::Ref<Kross::Camera::Ortho2DCtrl> camera;

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
};

