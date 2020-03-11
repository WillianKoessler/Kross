#pragma once
#include "Kross.h"

class Sandbox2D final : public Kross::Layer
{
	int w, h;
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Kross::Timestep ts) override;
	virtual void OnImGuiRender(Kross::Timestep ts) override;
	
	virtual void OnEvent(Kross::Event& e) override;
private:
	bool demo;
	float rotquad1, rotquad2, rotbg;
	Kross::Ref<Kross::Textures::T2D> texture;


	Kross::Camera::Ortho2DCtrl m_Camera;
	float pos[2] = { 2.0f, 0.0f };
	glm::vec4 color1 = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 color2 = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 bgcolor= { 1.0f, 1.0f, 1.0f, 1.0f };

	Kross::Ref<Kross::VertexArray> va;
};
