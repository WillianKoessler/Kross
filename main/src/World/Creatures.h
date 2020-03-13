#pragma once

#include "World/Entities.h"

class Creature final : public Entity
{

public:
	struct equipment
	{
		int* Rhand		= nullptr; // ref, scope, or weak?
		int* Lhand		= nullptr; // ref, scope, or weak?
		int* TopHead	= nullptr; // ref, scope, or weak?
		int* MiddleHead	= nullptr; // ref, scope, or weak?
		int* BottomHead	= nullptr; // ref, scope, or weak?
		int* Torso		= nullptr; // ref, scope, or weak?
		int* Legs		= nullptr; // ref, scope, or weak?
		int* Feets		= nullptr; // ref, scope, or weak?
		int* Accessory1	= nullptr; // ref, scope, or weak?
		int* Accessory2	= nullptr; // ref, scope, or weak?
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
	enum state
	{
		Standing,
		Sit,
		Walking,
		Dead
	} myState = Standing;
	enum direction
	{
		South = 0,
		SouthWest,
		West,
		NorthWest,
		North,
		NorthEast,
		East,
		SouthEast,
	} myDirection = South;
	long double timer = 0.0;
	size_t gfxCounter = 0;
	float sprite_speed = 0.87f;
	float dump = 0.7f;
	float speed = 0.7f;
	bool debugWindow = false;
	bool active = false;
	bool sit = false;

	//precision tolerance
	float pt = 0.01f;

	Creature(const Entity::Props& prop) :
		Entity(prop),
		mhp(100), hp(mhp)
	{}

	bool tgm(bool set);
	bool applyDamage(int amount, Creature* victim) const;
	bool receiveDamage(int amount, const Creature* attacker);
	void Input(float ts);
	virtual void OnUpdate(float ts) override;
	virtual void DrawSelf() override;
	void Log();
	void walk(Props& p);

	int mhp, hp;
	std::vector<Item*> inventory;
};
