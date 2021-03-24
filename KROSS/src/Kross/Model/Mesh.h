#pragma once
#include <vector>

namespace Kross {
	class Mesh
	{
	public:
		std::vector<float> positions;
		std::vector<float> texCoords;
		std::vector<uint32_t> indices;
	};
}