#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/Vector.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arboria {
	class Camera {
		public:
			Camera(Vector2<size_t> viewportSize);
			Camera(Vector2<size_t> viewportSize, Vector2<float> cameraPosition, float zoom = 1.0f, float maxZoomOut = 0.005f, float defaultZoomRotation = 1.0f);
			Camera(float x, float y, float bottom, float top);
			~Camera() = default;
			
			const Vector2<float>& getPosition() const { return cameraPosition; }

			const glm::mat4& getViewMatrix() const { return viewMatrix; }
			const glm::mat4& getViewProjection() const { return viewProjection; }
			glm::mat4& getViewMatrix() { return viewMatrix; }
			glm::mat4& getViewProjection() { return viewProjection; }
		protected:
			void recalculateViewMatrix();
		private:
			Vector2<size_t> viewportSize;
			Vector2<float> cameraPosition;
			float zoom;
			float maxZoomOut;
			float defaultZoomRatio;
			bool zoomChanged;
			bool viewPortChanged;
			bool moved;
			glm::mat4 viewMatrix;
			glm::mat4 viewProjection;
	};
}

#endif