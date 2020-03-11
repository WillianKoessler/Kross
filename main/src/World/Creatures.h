#pragma once

#include "World/Entities.h"

class Creature final : public Entity
{
	struct equipment
	{
		int* Rhand		= nullptr;
		int* Lhand		= nullptr;
		int* TopHead	= nullptr;
		int* MiddleHead	= nullptr;
		int* BottomHead	= nullptr;
		int* Torso		= nullptr;
		int* Legs		= nullptr;
		int* Feets		= nullptr;
		int* Accessory1	= nullptr;
		int* Accessory2	= nullptr;
	} eqp;
	struct status
	{
		short FOR = 1;
		short AGI = 1;
		short VIT = 1;
		short INT = 1;
		short DES = 1;
		short SOR = 1;
	} stt;
public:

	Creature(const Entity::Props& prop) :
		Entity(prop),
		mhp(100), hp(mhp)
	{}

	bool tgm(bool set);
	bool applyDamage(int amount, Creature* victim) const;
	bool receiveDamage(int amount, const Creature* attacker);
	void DrawSelf(float ts);
	void Log();

	int mhp, hp;
	std::vector<Item*> inventory;
};
