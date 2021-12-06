#include "EntityInspector.h"
namespace Kross {
	EntityInspector::EntityInspector(const char *name)
	{
		SetName(name);
		KROSS_INFO("Entity Inspector Panel '{0}' Constructed", name);
	}
	void EntityInspector::Show(double ts)
	{
	}
}