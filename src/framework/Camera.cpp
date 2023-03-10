#include "Camera.h"

namespace Arboria {
	Camera::Camera(float left, float right, float bottom, float top) {
		cameraPosition = Vector2<float>(0);
		viewProjection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		viewMatrix = glm::mat4(1);
	}

	void Camera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1), { cameraPosition.x, cameraPosition.y, 0.f });
		transform = glm::inverse(transform);
		viewMatrix = transform;
	}
}