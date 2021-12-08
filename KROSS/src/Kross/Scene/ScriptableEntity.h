#pragma once

#include "Kross/Core/Core.h"
#include "Entity.h"

namespace Kross {
	class ScriptableEntity
	{
	public:
		template<typename Component>
		Component &GetComponent()
		{
			return m_Entity.GetComponent<Component>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(double ts) {}
		virtual void OnDestroy() {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}