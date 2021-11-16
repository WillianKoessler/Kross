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
		Props(const glm::vec3& pos, const unsigned char efs, const std::string& name, const std::string& spr)
			:
			sprite(),
			spritePath(spr),
			pos(pos),
			EFs(efs),
			name(name)
		{
			sprite.position = pos;
		}
		Kross::QuadParams sprite;
		Kross::Ref<Kross::Texture::T2DAtlas> atlas;
		std::string spritePath = "";
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 vel = { 0.0f, 0.0f, 0.0f };
		glm::vec3 acc = { 0.0f, 0.0f, 0.0f };
		unsigned char EFs;
		const std::string name;
	};

	enum EF
	{
		None = BIT(0),
		Solid = BIT(1),
		Friendly = BIT(2),
		Flyable = BIT(3),
		Alive = BIT(4),
		GodMode = BIT(5),
	};

	Entity(const Props& props_ = { glm::vec3(0.0f) , EF::None, "Default Name", "" }) :
		props(props_)
	{}

	inline const unsigned char GetFlags(unsigned char mask) const { return props.EFs & mask; }
	inline const unsigned char GetFlags() const { return props.EFs; }
	inline const void SetFlagsOn(unsigned char mask) { props.EFs |= mask; }
	inline const void SetFlagsOff(unsigned char mask) { props.EFs &= ~mask; }
	inline const std::string& GetName() const { return props.name; }
	inline const glm::vec3& GetPos() const { return props.pos; }
	inline const glm::vec3& GetVel() const { return props.vel; }
	inline const float GetX() const { return props.pos.x; }
	inline const float GetY() const { return props.pos.y; }
	inline void SetAcc(const glm::vec3& acc) { props.acc = acc; }
	inline void SetPos(const glm::vec3& newpos) { props.pos = newpos; }
	inline void SetPos(const glm::vec2& newpos) { props.pos = { newpos.x, newpos.y, 0.0f }; }
	inline const Kross::QuadParams& GetSprite(const glm::vec2& index)
	{
		props.sprite.position = props.pos;
		props.sprite.subTexture->UpdateTexture(index);
		return props.sprite;
	}

	virtual ~Entity() = default;

	virtual void Input(float ts) {};
	virtual void OnUpdate(float ts) {};
	virtual bool interact() { return false; }
	virtual void DrawSelf() {}
	virtual void ConfigureAtlas(const glm::vec2& cellSize, const glm::vec2& spriteSize) {}

protected:
	inline Props& GetProps() { return props; }

private:
	Props props;
};
