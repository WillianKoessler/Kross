#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Input.h"
#include "Kross/Events/KeyEvent.h"

namespace Kross {
	class ActionManager
	{
	public:
		//template<typename FuncType>
		using Func = std::function<void(const void*, size_t)>;

		static bool IsActionPerformed(const char *action);

		static void RegisterAction(const char *actionIdentifier, Key special1, Key special2, Key key, Func function, const char *description = "");
		static void RegisterAction(const char *actionIdentifier, Key special, Key key, Func function, const char *description = "");
		static void RegisterKeyAction(const char *actionIdentifier, Key key, Func function, const char *description = "");
		
		static void RegisterAction(const char *actionIdentifier, Key special1, Key special2, Key key, const char *description = "");
		static void RegisterAction(const char *actionIdentifier, Key special, Key key, const char *description = "");
		static void RegisterKeyAction(const char *actionIdentifier, Key key, const char *description = "");
		
		static void ModifyAction(const char *actionIdentifier, Key special1, Key special2, Key key, const char *description = "");
		static void ModifyAction(const char *actionIdentifier, Key special, Key key, const char *description = "");
		static void ModifyKeyAction(const char *actionIdentifier, Key key, const char *description = "");
		
		static void TriggerAction(const char *actionIdentifier, const void* data, size_t size);
		static void EraseAction(const char *actionIdentifier);
		
		static void PoolEvent(KeyPressedEvent &e);
		static void PoolActions();
	};
}
