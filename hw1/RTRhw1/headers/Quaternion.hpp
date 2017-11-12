#ifndef _QUATERNION_HPP
#define _QUATERNION_HPP

#include <cmath>
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>

class quaternion
{
	public :

		float w, x, y, z;

		quaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
		quaternion(float a, float b, float c, float d) : w(a), x(b), y(c), z(d){}
		quaternion(glm::vec3 v, float angle)
		{
			float rad = angle * 180.0 / 3.14159;
			w = cos(rad * 0.5f);

			float s = sin(rad * 0.5f);
			x = v.x * s;
			y = v.y * s;
			z = v.z * s;
		}

		void mul(float f) 
		{
			w *= f;
			x *= f;
			y *= f;
			z *= f;
		}

		void Sub(quaternion q) 
		{
			w -= q.w;
			x -= q.x;
			y -= q.y;
			z -= q.z;
		}

		void Add(quaternion q) 
		{
			w += q.w;
			x += q.x;
			y += q.y;
			z += q.z;
		}

		void Normalize() {
			float len = 1.0 / sqrtf(w * w + x * x + y * y + z * z);
			mul(len);
		}

		quaternion inverse()
		{
			return quaternion(w, -x, -y, -z);
		}

		float dot(quaternion q)
		{
			return w * q.w + x * q.x + y * q.y + z * q.z;
		}

		quaternion mul(quaternion rhs)
		{
			quaternion tmpQ;

			glm::vec3 vThis(x, y, z);
			glm::vec3 vRhs(rhs.x, rhs.y, rhs.z);
			
			tmpQ.w = w * rhs.w - glm::dot(vThis, vRhs);

			glm::vec3 product = w * vRhs + rhs.w * vThis + glm::cross(vThis, vRhs);
			tmpQ.x = product.x;
			tmpQ.y = product.y;
			tmpQ.z = product.z;

			return tmpQ;
		}


		glm::vec3 rotate(glm::vec3 rhs)
		{
			// q * [0, r] * q.inv()
			quaternion result = mul(quaternion(0.0, rhs.x, rhs.y, rhs.z)).mul(this->inverse());
			return glm::vec3(rhs.x, rhs.y, rhs.z);
			// auto q = glm::quat(w, x, y, z);
			// return glm::rotate(q, rhs);
		}

		quaternion rotate(float angle, glm::vec3 rhs)
		{
			float sin = sinf(angle * 0.5f);
			float cos = cosf(angle * 0.5f);

			return quaternion(cos, rhs.x * sin, rhs.y * sin, rhs.z * sin);
		}
};

#endif // !_QUATERNION_HPP
