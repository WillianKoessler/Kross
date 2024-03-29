#include "Kross_pch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Kross::Math {
	bool DecomposeTransform(const glm::mat4 &inTransform, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale)
	{

		// From glm::decompose in matrix_decompose.inl

		using namespace glm;

		mat4 LocalMatrix(inTransform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<float>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(LocalMatrix[1][3], 0.0f, epsilon<float>()) ||
			epsilonNotEqual(LocalMatrix[2][3], 0.0f, epsilon<float>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = 0.0f; // 0  x  x  x
			LocalMatrix[1][3] = 0.0f; // x  0  x  x
			LocalMatrix[2][3] = 0.0f; // x  x  0  x
			LocalMatrix[3][3] = 1.0f; // x  x  x  1
		}

		// Next take care of translation (easy).
		outTranslation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		outScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], 1.0f);
		outScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], 1.0f);
		outScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], 1.0f);

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= -1.0f;
				Row[i] *= -1.0f;
			}
		}
#endif

		outRotation.y = asin(-Row[0][2]);
		if (cos(outRotation.y) != 0) {
			outRotation.x = atan2(Row[1][2], Row[2][2]);
			outRotation.z = atan2(Row[0][1], Row[0][0]);
		} else {
			outRotation.x = atan2(-Row[2][0], Row[1][1]);
			outRotation.z = 0;
		}


		return true;
	}
}
