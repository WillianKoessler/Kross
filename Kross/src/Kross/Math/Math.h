#pragma once
#include "Kross/Core/Core.h"
#include <glm/glm.hpp>

namespace Kross::Math {
	bool DecomposeTransform(const glm::mat4 &inTransform, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale);
}
