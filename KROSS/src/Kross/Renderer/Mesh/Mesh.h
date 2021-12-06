#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Renderer.h"
#include "Kross/Core/Resource.h"

namespace Kross::Mesh {
	class Base : public Resource
	{
	public:
		Base() { SetName("Unnamed_MeshBase"); }
	};
}