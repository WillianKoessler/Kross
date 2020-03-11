#include "Creatures.h"

#include <iostream>
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
	if(!victim)
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

void Creature::DrawSelf(float ts)
{

	//Kross::Renderer2D::DrawQuad(GetPos(), 1, GetSprite());
	//Kross::Renderer2D::DrawQuad(props.pos, 1, Kross::Renderer::Get2DTexStack().Get(props.spr));
}

void Creature::Log()
{
	std::string log("");
	if (GetFlags(Entity::EF::Alive))
	{
		log += "\nName: " + GetName();
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
		log += "\nHealth: " + hp + '/' + mhp;
		//std::cout << "Mana: " << sp << "/" << msp << std::endl;
		//std::cout << "Weight: " << w << "/" << mw << std::endl;
	}
	else
	{
		log += '\n' + GetName() + " is dead!!";
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
	}
	KROSS_TRACE(log);
}