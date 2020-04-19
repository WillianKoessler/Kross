#include "pch.h"
#include "Creatures.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

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

void Creature::Input()
{
	if (active)
	{
		auto& p = GetProps();
		if (Kross::Input::IsMouseButtonPressed(KROSS_MOUSE_BUTTON_1))
		{
			//glm::vec3 mouse = { Kross::Input::GetMousePos() , 0.0f};
			//glm::vec3 cam = { camera.GetSize().x * camera.GetZoom(), camera.GetSize().y * camera.GetZoom(), 0.0f};
			//p.acc = camera.GetCamera()->GetPosition() + mouse - cam;
			//p.acc /= sqrt(p.acc.x * p.acc.x + p.acc.y * p.acc.y);
			//p.acc *= ts;
		}
		p.acc.y = (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN));
		p.acc.x = (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT));
		if (Kross::Input::IsKeyReleased(KROSS_KEY_INSERT)) sit = !sit;
	}
}

void Creature::OnUpdate(float ts)
{
	timer += ts;
	if (timer >= (1 - sprite_speed))
	{
		timer -= (1 - sprite_speed);
		gfxCounter++;
		gfxCounter %= 8;
	}

	auto& p = GetProps();

	if (!sit)
	{
		p.vel += p.acc * ts;
		p.vel *= dump * speed;
		if (abs(p.vel.x) < pt) p.vel.x = 0;
		if (abs(p.vel.y) < pt) p.vel.y = 0;
		p.pos += p.vel;
	}

	if (p.vel.x || p.vel.y)
		myState = Walking;
	else
		myState = Standing;

	if (sit) myState = Sit;

	if (hp <= 0)
		myState = Dead;

#define eight_directions 1
#if eight_directions
	if (p.acc.x == 0.0f && p.acc.y < 0.0f) myDirection = South;
	else if (p.acc.x > 0.0f && p.acc.y < 0.0f) myDirection = SouthEast;
	else if (p.acc.x > 0.0f && p.acc.y == 0.0f) myDirection = East;
	else if (p.acc.x > 0.0f && p.acc.y > 0.0f) myDirection = NorthEast;
	else if (p.acc.x == 0.0f && p.acc.y > 0.0f) myDirection = North;
	else if (p.acc.x < 0.0f && p.acc.y >  0.0f) myDirection = NorthWest;
	else if (p.acc.x < 0.0f && p.acc.y == 0.0f) myDirection = West;
	else if (p.acc.x < 0.0f && p.acc.y < 0.0f) myDirection = SouthWest;

#else
	if (abs(p.vel.y) > pt)
	{
		if (p.acc.y < -pt) myDirection = South;
		if (p.acc.y > pt) myDirection = North;
	}
	if (abs(p.acc.x) > pt)
	{
		if (p.acc.x < -pt) myDirection = West;
		if (p.acc.x > pt) myDirection = East;
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
		switch (myDirection)
		{
		case East: sprite.texOffSet = { West * sub.x, 1.0f - sub.y }; sprite.FlipX(); break;
		case NorthEast: sprite.texOffSet = { NorthWest * sub.x, 1.0f - sub.y }; sprite.FlipX(); break;
		case SouthEast: sprite.texOffSet = { SouthWest * sub.x, 1.0f - sub.y }; sprite.FlipX(); break;
		default: sprite.texOffSet = { myDirection * sub.x, 1.0f - sub.y }; break;
		}
		break;
	}
	case Sit:
	{
		switch (myDirection)
		{
		case East: sprite.texOffSet = { West * sub.x + sitting_offset, 1.0f - sub.y }; sprite.FlipX(); break;
		case NorthEast: sprite.texOffSet = { NorthWest * sub.x + sitting_offset, 1.0f - sub.y }; sprite.FlipX(); break;
		case SouthEast: sprite.texOffSet = { SouthWest * sub.x + sitting_offset, 1.0f - sub.y }; sprite.FlipX(); break;
		default: sprite.texOffSet = { myDirection * sub.x + sitting_offset, 1.0f - sub.y }; break;
		}
		break;
	}
	case Walking:
	{
		switch (myDirection)
		{
		case East: sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - West * sub.y }; sprite.FlipX(); break;
		case NorthEast: sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - NorthWest * sub.y }; sprite.FlipX(); break;
		case SouthEast: sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - SouthWest * sub.y }; sprite.FlipX(); break;
		default: sprite.texOffSet = { (float)gfxCounter * sub.x , walking_offset - myDirection * sub.y }; break;
		}
		break;
	}
	case Dead:
		//spr = { 4, 1 };
		break;
	}

	sprite.position = { p.pos.x - sprite.size.x / 2.0f, p.pos.y - 0.2f };
	Kross::Renderer2D::BatchQuad(sprite);
}

void Creature::ShowDebugWindow()
{
	if (!debugWindow) return;
	if(!ImGui::Begin(GetName().c_str(), &debugWindow))
	{
		ImGui::End();
		return;
	}
	auto& p = GetProps();
	ImGui::Text("Position: X=%.1f, Y=%.1f", p.pos.x, p.pos.y);
	ImGui::Text("Velocity: X=%.10f, Y=%.10f", p.vel.x, p.vel.y);
	ImGui::Text("HP %d/%d", hp, mhp);
	ImGui::Text("State: %d", myState);
	ImGui::Text("Direction: %d", myDirection);
	ImGui::Checkbox("Inputs", &active);
	ImGui::SliderFloat("Speed", &speed, 0.0f, 1.0f);
	ImGui::SliderFloat("sprite_speed", &sprite_speed, 0.0f, 1.0f);
	ImGui::SliderFloat("dump", &dump, 0.0f, 1.0f);
	ImGui::SliderFloat2("vel", glm::value_ptr(p.vel), -.1f, .1f);
	ImGui::End();
}

void Creature::Log()
{
	std::string log("");
	if (GetFlags(Entity::EF::Alive))
	{
		log += "\nName: " + GetName();
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
		log += "\nHealth: " + hp + '/' + mhp;
	}
	else
	{
		log += '\n' + GetName() + " is dead!!";
		log += "\nPosition: " + std::to_string(GetX()) + ", " + std::to_string(GetY());
	}
	KROSS_TRACE(log);
}

void Creature::walk(Props& p)
{
	p.pos += p.acc * speed;
}
