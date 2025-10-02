#include "Camera.h"

namespace Arboria {
	Camera::Camera(Vector2<size_t> viewportSize) :
		viewportSize(viewportSize),
		cameraPosition({ 0.f, 10.f }),
		zoom(1.0f),
		maxZoomOut(0.005f),
		defaultZoomRatio(1.f),
		moved(true),
		zoomChanged(true),
		viewPortChanged(false),
		viewMatrix({1.0f}),
		viewProjection({1.0f})
	{}

	Camera::Camera(Vector2<size_t> viewportSize, Vector2<float> cameraPosition, float zoom, float maxZoomOut, float defaultZoomRotation) :
		viewportSize(viewportSize),
		cameraPosition(cameraPosition),
		zoom(zoom),
		maxZoomOut(maxZoomOut),
		defaultZoomRatio(defaultZoomRotation),
		moved(true),
		zoomChanged(true),
		viewPortChanged(true),
		viewMatrix({ 1.0f }),
		viewProjection({ 1.0f })
	{}

	Camera::Camera(float left, float right, float bottom, float top) {
		cameraPosition = Vector3<float>(0);
		viewProjection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		viewMatrix = glm::mat4(1);
	}

	void Camera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1), { cameraPosition.x, cameraPosition.y, 0.f });
		transform = glm::inverse(transform);
		viewMatrix = transform;
	}
}