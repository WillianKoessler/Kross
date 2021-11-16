#pragma once
#include "Kross.h"

class world : public Kross::Layer
{
	// main camera
	Kross::Ref<Kross::Camera::Controller> m_Camera;
public:
	world();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Kross::Timestep) override;
	virtual void OnImGuiRender(Kross::Timestep) override;
	virtual void OnEvent(Kross::Event& event) override;
};

