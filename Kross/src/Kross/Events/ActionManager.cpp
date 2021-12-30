#include "Kross_pch.h"
#include "ActionManager.h"

#define IsSomething(key, thing) (key == Key::##thing || key == Key::Left##thing || key == Key::Right##thing)
#define IsControl(key) IsSomething(key, Control)
#define IsShift(key) IsSomething(key, Shift)
#define IsAlt(key) IsSomething(key, Alt)
#define IsSpecial(key) (IsControl(key) || IsShift(key) || IsAlt(key))

#define NotSomething(key, thing) (key != Key::##thing && key != Key::Left##thing && key != Key::Right##thing)
#define NotControl(key) NotSomething(key, Control)
#define NotShift(key) NotSomething(key, Shift)
#define NotAlt(key) NotSomething(key, Alt)
#define NotSpecial(key) (NotControl(key) && NotShift(key) && NotAlt(key))

#define HasSomething(thing) bool has##thing = false; for(int i = 1; i < 3; i++) has##thing |= Is##thing(action.keys[i]);

namespace Kross {
	struct Action
	{
		std::string id = "";
		bool activate = false;
		Key keys[3] = { Key::None };
		std::string description = "";
		ActionManager::Func function;
		void *data = nullptr;
		size_t size = 0;
		bool operator==(const Action &other) const
		{
			return
				keys[0] == other.keys[0] &&
				keys[1] == other.keys[1] &&
				keys[2] == other.keys[2];
		}
	};
	static std::vector<Action> actions;
	static Action pooledAction;
	static auto location(const Action &action)
	{
		//for (auto i = actions.begin(); i != actions.end(); i++)
		//	if ((*i).id == action.id) return i;
		//return actions.end();
		//return std::find(actions.begin(), actions.end(), action);
		return std::lower_bound(actions.begin(), actions.end(), action, [](const Action &a, const Action &b) { return strcmp(a.id.c_str(), b.id.c_str()) >= 0; });
	}
	static auto location(const std::string &action)
	{
		Action temp;
		temp.id = action;
		return location(temp);
	}
	static bool validate(const char *id, Key &special1, Key &special2, Key &key)
	{
		int countSpecial = 0;
		countSpecial += IsSpecial(special1) || special1 == Key::None;
		countSpecial += IsSpecial(special2) || special2 == Key::None;
		countSpecial += IsSpecial(key);
		if (key == Key::None) countSpecial = 3;
		if (special1 == special2) special2 = Key::None;
		if ((special1 == Key::None && special2 != Key::None)) {
			special1 = special2;
			special2 = Key::None;
		}
		if ((special1 != Key::Control && special2 == Key::Control) ||
			(special1 == Key::Shift && special2 == Key::Alt)) {
			Key temp = special1;
			special1 = special2;
			special2 = temp;
		}
		switch (countSpecial) {
			case 0: return true;
			case 1: KROSS_WARN("'{0}' Action action can only have ONE non special key", id); break;
			case 2: return true;
			case 3: KROSS_WARN("'{0}' Action cannot have just special keys", id); break;
		}
		return false;
	}
	static void emplace(const Action &act)
	{
		auto i = location(act.id);
		if (i != actions.end() && (*i).id == act.id) KROSS_WARN("Action '{0}' already exist", act.id);
		else {
			KROSS_INFO("Action '{0}' registered. ({1}, {2}, {3})", act.id, (int)act.keys[0], (int)act.keys[1], (int)act.keys[2]);
			actions.emplace(i, act);
		}
	}
	bool ActionManager::IsActionPerformed(const char *action)
	{
		for (auto &a : actions) if (a.id == action) return a.activate;
		auto i = location(action);
		if (i != actions.end() && strcmp((*i).id.c_str(), action) == 0)
			return (*i).activate;
		return false;
	}
	void ActionManager::RegisterAction(const char *id, Key special1, Key special2, Key key, Func function, const char *description)
	{
		if (!validate(id, special1, special2, key)) return;
		Action act;
		act.id = std::string(id);
		act.keys[0] = key;
		act.keys[1] = special1;
		act.keys[2] = special2;
		act.function = function;
		act.description = description;
		emplace(act);
	}
	void ActionManager::RegisterAction(const char *id, Key special, Key key, Func function, const char *description)
	{
		RegisterAction(id, Key::None, special, key, function, description);
	}
	void ActionManager::RegisterKeyAction(const char *id, Key key, Func function, const char *description)
	{
		RegisterAction(id, Key::None, Key::None, key, function, description);
	}
	void ActionManager::RegisterAction(const char *id, Key special1, Key special2, Key key, const char *description)
	{
		RegisterAction(id, special1, special2, key, [](const void *data, size_t size) {}, description);
	}
	void ActionManager::RegisterAction(const char *id, Key special, Key key, const char *description)
	{
		RegisterAction(id, special, Key::None, key, [](const void *data, size_t size) {}, description);
	}
	void ActionManager::RegisterKeyAction(const char *id, Key key, const char *description)
	{
		RegisterAction(id, Key::None, Key::None, key, [](const void* data, size_t size) {}, description);
	}
	void ActionManager::ModifyAction(const char *id, Key special1, Key special2, Key key, const char *description)
	{
		auto i = location(id);
		if (i != actions.end() && strcmp((*i).id.c_str(), id) == 0) {
			if (validate(id, special1, special2, key)) {
				i->keys[0] = key;
				i->keys[1] = special1;
				i->keys[2] = special2;
				i->description = description;
			}
		} else KROSS_WARN("Action '{0}' was not found.", id);
	}
	void ActionManager::ModifyAction(const char *id, Key special, Key key, const char *description)
	{
		ModifyAction(id, special, Key::None, key, description);
	}
	void ActionManager::ModifyKeyAction(const char *id, Key key, const char *description)
	{
		ModifyAction(id, Key::None, Key::None, key, description);
	}
	void ActionManager::TriggerAction(const char *id, const void* data, size_t size)
	{
		auto i = location(id);
		if (i != actions.end()) {
			if (i != actions.begin()) i--;
			if (strcmp((*i).id.c_str(), id) == 0) {
				(*i).activate = true; // TODO: turn this to 'activate' the action at end/begin of the frame
				(*i).data = malloc(size);
				memcpy((*i).data, data, size);
				(*i).size = size;
				KROSS_TRACE("set for trigger");
				if (data != nullptr) KROSS_TRACE("Has Data pointer");
				if (size > 0) KROSS_TRACE("Has Data size");
			} else {
				KROSS_WARN("Action '{0}' did not match '{1}'", (*i).id, id);
			}
		} else KROSS_WARN("Action '{0}' was not found.", id);
	}
	void ActionManager::EraseAction(const char *id)
	{
		auto i = location(id);
		if (i != actions.end() && strcmp((*i).id.c_str(), id) == 0)
			actions.erase(i);
		else KROSS_WARN("Action '{0}' was not found.", id);
	}
	void ActionManager::PoolActions()
	{
		for (auto &action : actions) {
			if (action.activate && action.data != nullptr && action.size > 0) {
				KROSS_TRACE("calling action::function");
				action.function(action.data, action.size);
				free(action.data);
				action.data = nullptr;
				action.size = 0;
			}
			action.activate = action == pooledAction;
		}
		pooledAction = {};
	}
	void ActionManager::PoolEvent(KeyPressedEvent &e)
	{
		uint8_t specials = Input::IsKeyHeld(Key::Control) << 0 | Input::IsKeyHeld(Key::Shift) << 1 | Input::IsKeyHeld(Key::Alt) << 2;
		switch (specials) {
			case 1:  pooledAction.keys[1] = Key::Control; break;
			case 2:  pooledAction.keys[1] = Key::Shift;	break;
			case 3:  pooledAction.keys[1] = Key::Control; pooledAction.keys[2] = Key::Shift; break;
			case 4:  pooledAction.keys[1] = Key::Alt;     break;
			case 5:  pooledAction.keys[1] = Key::Control; pooledAction.keys[2] = Key::Alt; break;
			case 6:  pooledAction.keys[1] = Key::Alt;     pooledAction.keys[2] = Key::Shift; break;
		}
		pooledAction.keys[0] = e.GetKey();
	}
}
