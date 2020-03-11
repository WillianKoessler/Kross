#include "pch.h"
#include "Creatures.h"

#include "Kross.h"

bool Creature::tgm(bool set)
{

	if (GetFlags(Entity::EF::GodMode))
	{
		SetFlagsOff(Entity::EF::GodMode);
		return false;
	}
	else
	{
		SetFlagsOn(Entity::EF::GodMode);
		return true;
	}
}

bool Creature::applyDamage(int amount, Creature* victim) const
{
	if (!victim)
	{
		char buff[256];
		strcpy_s(buff, GetName().c_str());
		strcat_s(buff, " have no victim. (Creature* victim == nullptr)");
		KROSS_MSGBOX(buff, __FUNCSIG__, _ERROR_);
		return false;
	}
	else
	{
		KROSS_TRACE("{0} attacks {1}", GetName(), victim->GetName());
		if (GetFlags(Entity::EF::GodMode & Entity::EF::Alive))
			return false;
		else
		{
			KROSS_TRACE("{0} damage.", amount);
			victim->receiveDamage(amount, this);// amount / victim.def * this->atq;
		}
		return true;
	}
}

bool Creature::receiveDamage(int amount, const Creature* attacker)
{
	if (hp <= 0) SetFlagsOff(Entity::EF::Alive);
	return hp <= 0;
}

void Creature::OnUpdate(float ts)
{
	timer += ts;
	if (timer >= 0.07)
	{
		timer -= 0.07;
		gfxCounter++;
		gfxCounter %= 9;
	}

	auto& p = GetProps();
	p.vel += p.acc;
	p.vel *= 0.7f;
	p.pos += p.vel;


	
	if (fabsf(p.vel.x) < 0.01f && fabsf(p.vel.y) < 0.01f)
		myState = Standing;
	else
		myState = Walking;

	if (hp <= 0)
		myState = Dead;


	//if (v->x == 0.0f && v->y < 0.0f) myDirection = South;
	//else if (v->x > 0.0f && v->y < 0.0f) myDirection = SouthEast;
	//else if (v->x > 0.0f && v->y == 0.0f) myDirection = East;
	//else if (v->x > 0.0f && v->y > 0.0f) myDirection = NorthEast;
	//else if (v->x == 0.0f && v->y > 0.0f) myDirection = North;
	//else if (v->x < 0.0f && v->y > 0.0f) myDirection = NorthWest;
	//else if (v->x < 0.0f && v->y == 0.0f) myDirection = West;
	//else if (v->x < 0.0f && v->y < 0.0f) myDirection = SouthWest;
	if (!(p.vel.y < 0.001f && p.vel.y > -0.001f))
	{
		if (p.vel.y < 0.0001f) myDirection = North;
		if (p.vel.y > 0.0001f) myDirection = South;
	}
	if (!(p.vel.x < 0.001f && p.vel.x > -0.001f))
	{
		if (p.vel.x < 0.0001f) myDirection = West;
		if (p.vel.x > 0.0001f) myDirection = East;
	}
}

void Creature::DrawSelf()
{
	glm::vec2 spr(0.0f);
	switch (myState)
	{
	case Walking:
		spr = { (float)gfxCounter, (float)myDirection };
		break;
	case Standing:
		spr = { 0.0f, (float)myDirection };
		break;
	case Dead:
		spr = { 4, 1 };
		break;
	}

	Kross::Renderer2D::BatchQuad(GetSprite(
		{ (spr.x * SPRITE_SIZE) / 576, (spr.y * SPRITE_SIZE) / 256 },
		{ SPRITE_SIZE / 576, SPRITE_SIZE / 256 }));
}

void Creature::Log()
{
	std::string log("");
	if (GetFlags(Entity::EF::Alive))
	{
		log += "\nName: " + GetName();
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
		log += "\nHealth: " + hp + '/' + mhp;
		//log += "\nMana: " + sp + '/' + msp;
		//log += "\nWeight: " + w + '/' + mw;
	}
	else
	{
		log += '\n' + GetName() + " is dead!!";
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
	}
	KROSS_TRACE(log);
}