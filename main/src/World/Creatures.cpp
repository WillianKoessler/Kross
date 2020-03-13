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
	if (!victim) { KROSS_MSGBOX(GetName() + " have no victim. (Creature* victim == nullptr)", __FUNCTION__, _ERROR_); return false; }
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

void Creature::Input(float ts)
{
	if (active)
	{
		static bool previous_sit = true;
		auto& p = GetProps();
		p.acc.y = (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN)) * ts;
		p.acc.x = (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT)) * ts;
		if (Kross::Input::IsKeyPressed(KROSS_KEY_INSERT) && sit != previous_sit)
		{
			Kross::Input::IsKeyPressed
			previous_sit = sit;
			sit = !sit;
		}
	}
}

void Creature::OnUpdate(float ts)
{
	Input(ts);
	timer += ts;
	if (timer >= (1 - sprite_speed))
	{
		timer -= (1 - sprite_speed);
		gfxCounter++;
		gfxCounter %= 8;
	}

	auto& p = GetProps();
	//p.vel += p.acc;
	//p.vel *= dump;
	p.pos += p.acc;



	if (fabsf(p.acc.x) < 0.0000000001f && fabsf(p.acc.y) < 0.0000000001f)
		myState = Standing;
	else
		myState = Walking;

	if (sit) myState = Sit;

	if (hp <= 0)
		myState = Dead;

#define eight_directions 0
#if eight_directions
	if (p.vel.x == 0.0f && p.vel.y < 0.0f) myDirection = South;
	else if (p.vel.x > 0.0f && p.vel.y < 0.0f) myDirection = SouthEast;
	else if (p.vel.x > 0.0f && p.vel.y == 0.0f) myDirection = East;
	else if (p.vel.x > 0.0f && p.vel.y > 0.0f) myDirection = NorthEast;
	else if (p.vel.x == 0.0f && p.vel.y > 0.0f) myDirection = North;
	else if (p.vel.x < 0.0f && p.vel.y >  0.0f) myDirection = NorthWest;
	else if (p.vel.x < 0.0f && p.vel.y == 0.0f) myDirection = West;
	else if (p.vel.x < 0.0f && p.vel.y < 0.0f) myDirection = SouthWest;
#else
	if (!(p.acc.y < 0.0000000001f && p.acc.y > -0.0000000001f))
	{
		if (p.acc.y < -0.0000000001f) myDirection = South;
		if (p.acc.y > 0.0000000001f) myDirection = North;
	}
	if (!(p.acc.x < 0.0000000001f && p.acc.x > -0.0000000001f))
	{
		if (p.acc.x < -0.0000000001f) myDirection = West;
		if (p.acc.x > 0.0000000001f) myDirection = East;
	}
#endif
}

void Creature::DrawSelf()
{
	auto& p = GetProps();
	auto& sprite = GetSprite();

	constexpr glm::vec2 ind = { 100.0f, 118.0f };
	constexpr glm::vec2 sh = { 1003.0f, 1911.0f };
	constexpr glm::vec2 sub = ind / sh;
	constexpr float min_ = std::min(ind.x, ind.y), max_ = std::max(ind.x, ind.y);
	constexpr float min_norm = min_ / max_;
	constexpr float walking_offset = 1.0f - sub.y * 2;
	constexpr float sitting_offset = 5 * sub.x;

	sprite.texture = 0;
	sprite.size = { 0.1f, 0.1f };
	sprite.position = p.pos;
	Kross::Renderer2D::BatchQuad(sprite);

	sprite.texture = p.tex;
	sprite.texSubSize = sub;
	if (min_ == ind.x)
		sprite.size = { min_norm, 1.0f };
	else
		sprite.size = { 1.0f, min_norm };

	switch (myState)
	{
	case Standing:
	{
		if (myDirection == East)
		{
			sprite.texOffSet = { West * sub.x, 1.0f - sub.y };
			sprite.FlipX();
		}
		else sprite.texOffSet = { myDirection * sub.x, 1.0f - sub.y };
		break;
	}
	case Sit:
	{
		if (myDirection == East)
		{
			sprite.texOffSet = { West * sub.x + sitting_offset, 1.0f };
			sprite.FlipX();
		}
		else sprite.texOffSet = { myDirection * sub.x + sitting_offset, 1.0f };
		break;
	}
	case Walking:
	{
		if (myDirection == East)
		{
			sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - West * sub.y };
			sprite.FlipX();
		}
		else sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - myDirection * sub.y};
		break;
	}
	case Dead:
		//spr = { 4, 1 };
		break;
	}

	sprite.position = { p.pos.x - sprite.size.x / 2.0f, p.pos.y - 0.2f};
	Kross::Renderer2D::BatchQuad(sprite);

	//Kross::Renderer2D::BatchQuad(GetSprite(
	//	{ (spr.x * SPRITE_SIZE) / 576, (spr.y * SPRITE_SIZE) / 256 },
	//	{ SPRITE_SIZE / 576, SPRITE_SIZE / 256 }));
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