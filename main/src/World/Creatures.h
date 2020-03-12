#pragma once

#include "World/Entities.h"

class Creature final : public Entity
{
#define SPRITE_SIZE 64.0f
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
		Walking,
		Dead
	} myState = Standing;
	enum direction
	{
		East = 0,
		North,
		West,
		South,
		//SouthEast,
		//NorthEast,
		//NorthWest,
		//SouthWest,
	} myDirection = South;
	long double timer = 0.0;
	size_t gfxCounter = 0;
	float sprite_speed = 0.955f;
	float dump = 0.7f;
	bool debugWindow = false;

	Creature(const Entity::Props& prop) :
		Entity(prop),
		mhp(100), hp(mhp)
	{}

	bool tgm(bool set);
	bool applyDamage(int amount, Creature* victim) const;
	bool receiveDamage(int amount, const Creature* attacker);
	virtual void OnUpdate(float ts) override;
	virtual void DrawSelf() override;
	void Log();

	int mhp, hp;
	std::vector<Item*> inventory;
};
