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
			pos({pos, 0.0f}),
			EFs(efs),
			name(name),
			sprite()
		{
			sprite.position = pos;
			sprite.texture = Kross::Stack<Kross::Texture::T2D>::get().Get(Kross::FileName(spr), spr);
		}
		Kross::QuadParams sprite;
		glm::vec3 pos;
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

	inline const unsigned char GetFlags(unsigned char mask) const { return props.EFs & mask; }
	inline const unsigned char GetFlags() const { return props.EFs; }
	inline const void SetFlagsOn(unsigned char mask) { props.EFs |= mask; }
	inline const void SetFlagsOff(unsigned char mask) { props.EFs &= ~mask; }
	inline const std::string& GetName() const { return props.name; }
	inline const glm::vec3& GetPos() const { return props.pos; }
	inline const float GetX() const { return props.pos.x; }
	inline const float GetY() const { return props.pos.y; }
	inline void SetPos(const glm::vec3& newpos) { props.pos = newpos; }
	inline void SetPos(const glm::vec2& newpos) { props.pos = { newpos.x, newpos.y, 0.0f }; }
	inline const Kross::QuadParams& GetSprite(const glm::vec2& texoff = { 0.0f, 0.0f }, const glm::vec2& texsize = { 1.0f,1.0f })
	{
		props.sprite.texOffSet = texoff;
		props.sprite.texSubSize = texsize;
		return props.sprite;
	}

	virtual ~Entity() = default;

	virtual bool interact() { return false; }
	virtual void DrawSelf(float ox, float oy) {}

private:
	Props props;
};
