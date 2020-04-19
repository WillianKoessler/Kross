#pragma once

#include "glm/glm/glm.hpp"
#include <string>
#include <vector>

#include "Core/Items.h"
#include "Kross.h"

class Entity
{
public:
	struct Props {
		Props(const glm::vec2& pos, const unsigned char efs, const std::string& name, const std::string& spr)
			:
			sprite(),
			tex(Kross::Stack<Kross::Texture::T2D>::get().Get(Kross::FileName(spr), spr)),
			pos({pos, 0.0f}),
			EFs(efs),
			name(name)
		{
			sprite.color = glm::vec4(1.0f);
			sprite.position = pos;
			sprite.texture = tex;
		}
		Kross::QuadParams sprite;
		Kross::Ref<Kross::Texture::T2D> tex;
		glm::vec3 pos = {0.0f, 0.0f, 0.0f};
		glm::vec3 vel = {0.0f, 0.0f, 0.0f};
		glm::vec3 acc = {0.0f, 0.0f, 0.0f};
		unsigned char EFs;
		const std::string name;
	};

	enum EF
	{
		None		= BIT(0),
		Solid		= BIT(1),
		Friendly	= BIT(2),
		Flyable		= BIT(3),
		Alive		= BIT(4),
		GodMode		= BIT(5),
	};

	Entity(const Props& props_ = { {0, 0} , EF::None, "Default Name", ""}) :
		props(props_)
	{}

	inline const auto GetFlags(unsigned char mask) const { return props.EFs & mask; }
	inline const auto GetFlags() const { return props.EFs; }
	inline const void SetFlagsOn(unsigned char mask) { props.EFs |= mask; }
	inline const void SetFlagsOff(unsigned char mask) { props.EFs &= ~mask; }
	inline const auto& GetName() const { return props.name; }
	inline const auto& GetPos() const { return props.pos; }
	inline const auto& GetVel() const { return props.vel; }
	inline const auto GetX() const { return props.pos.x; }
	inline const auto GetY() const { return props.pos.y; }
	inline void SetAcc(const glm::vec3& acc) { props.acc = acc; }
	inline void SetPos(const glm::vec3& newpos) { props.pos = newpos; }
	inline void SetPos(const glm::vec2& newpos) { props.pos = { newpos.x, newpos.y, 0.0f }; }
	inline auto& GetSprite(const glm::vec2& texoff = { 0.0f, 0.0f }, const glm::vec2& texsize = { 1.0f,1.0f })
	{
		props.sprite.Reset();
		return props.sprite;
	}

	virtual ~Entity() = default;

	virtual void OnUpdate(float ts) {};
	virtual bool interact() { return false; }
	virtual void DrawSelf() {}
	virtual void ShowDebugWindow() {};

//protected:
	inline Props& GetProps() { return props; }

private:
	Props props;
};
