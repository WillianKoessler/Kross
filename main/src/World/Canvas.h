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
	Kross::Camera::Ortho2DCtrl camera;
};

