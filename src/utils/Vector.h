#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "math.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Arboria {

	template<typename T> using Vector4 = glm::tvec4<T, glm::highp>;
	template<typename T> using Vector3 = glm::tvec3<T, glm::highp>;
	template<typename T> using Vector2 = glm::tvec2<T, glm::highp>;
};

#endif