#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/Vector.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arboria {
	class Camera {
		public:
			Camera();
			Camera(float x, float y, float bottom, float top);

			inline void setPosition(Vector2<float> position) { cameraPosition = position; recalculateViewMatrix(); }
			const Vector2<float>& getPosition() const { return cameraPosition; }

			const glm::mat4& getViewMatrix() const { return viewMatrix; }
			const glm::mat4& getViewProjection() const { return viewProjection; }
			glm::mat4& getViewMatrix() { return viewMatrix; }
			glm::mat4& getViewProjection() { return viewProjection; }
		protected:
			void recalculateViewMatrix();
		private:
			Vector2<float> cameraPosition;
			glm::mat4 viewMatrix;
			glm::mat4 viewProjection;
	};
}

#endif