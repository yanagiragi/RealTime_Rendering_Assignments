#ifndef  _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

class Camera 
{
	public:
		vec3 cameraPos;
		vec3 cameraFront;
		vec3 cameraRight;

		float fov, nearPlane, farPlane, aspect_ratio;

		Camera() 
		{
			// Initial Values
			vec3 cameraPos = vec3(0.0, 0.0, 40.0);
			vec3 cameraFront = vec3(0.0, 0.0, -1.0);
			vec3 cameraUp = vec3(0.0, 1.0, 0.0);

			float fov = 45.0f;
			float aspect = 3.0 / 4.0f;
			float nearDistance = 0.1f;
			float farDistance = 1000.0f;

			Camera(cameraPos, cameraFront, cameraUp, fov, nearDistance, farDistance, aspect);
		}
		Camera(vec3 pos, vec3 front, vec3 right, float field, float n, float f, float ratio) 
		{
			cameraPos = pos;
			cameraFront = front;
			cameraRight = right;
			fov = field;
			nearPlane = n;
			farPlane = f;
			aspect_ratio = ratio;
		}

		glm::mat4 getV()
		{
			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraRight);
		}

		glm::mat4 getP()
		{
			return glm::perspective(glm::radians(fov), aspect_ratio, nearPlane, farPlane);
		}
};

#endif // ! _CAMERA_HPP
