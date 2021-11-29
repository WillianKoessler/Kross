#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Textures.h"
#include "Kross/Renderer/Textures/Atlas.h"

namespace Kross {
	struct QuadParams
	{
		glm::vec3				position = glm::vec3(0.0f);
		glm::vec2				size = glm::vec2(1.0f);
		Ref<Texture::T2D>		texture = nullptr;
		Ref<Texture::T2DAtlas>	subTexture = nullptr;
		glm::vec4				color = glm::vec4(1.0f);
		glm::vec3				rotation = glm::vec3(0.0f);
	};
	struct Params3D
	{
		glm::vec3				position = glm::vec3(0.0f);
		glm::vec3				size = glm::vec3(1.0f);
		glm::vec4				color = glm::vec4(1.0f);
		glm::vec3				rotation = glm::vec3(0.0f);
		Ref<Texture::T2D>		frontTexture = nullptr;
		Ref<Texture::T2D>		rightTexture = nullptr;
		Ref<Texture::T2D>		leftTexture = nullptr;
		Ref<Texture::T2D>		backTexture = nullptr;
		Ref<Texture::T2D>		topTexture = nullptr;
		Ref<Texture::T2D>		bottomTexture = nullptr;
	};
	struct CubeParams
	{
		glm::vec3				position = glm::vec3(0.0f);
		glm::vec3				size = glm::vec3(1.0f);
		glm::vec4				color = glm::vec4(1.0f);
	};
	struct Voxel
	{
		enum Faces
		{
			Bottom = 0x20,
			Top = 0x10,
			Right = 0x08,
			Back = 0x04,
			Left = 0x02,
			Front = 0x01,
			None = 0x00
		};

		glm::vec<3, uint8_t, glm::defaultp>		position = { 0u, 0u, 0u };
		glm::vec<4, uint8_t, glm::defaultp>		color = { 255u, 255u, 255u, 255u };
		uint8_t									visibleFaces = 0u;
	};
}