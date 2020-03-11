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
	float size = 10;
	Kross::Camera::Ortho2DCtrl camera;
	unsigned int uva, uvb, uib;
	Kross::Ref<Kross::Shader> shader;
	Kross::Ref<Kross::VertexArray> va;
	Kross::Ref<Kross::Buffer::Vertex> vb;
	Kross::Ref<Kross::Buffer::Index> ib;
	std::vector<Creature> entities;
	glm::vec2 pos, vel, acc;
};

