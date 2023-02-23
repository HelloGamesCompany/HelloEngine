#pragma once
#include "Globals.h"

namespace math
{
	class float2;
	class float3;
}

namespace API
{
	class API_Vector2;

	class TO_API API_Vector3
	{
	public:
		API_Vector3();

		API_Vector3(const float& x, const float& y, const float& z);

		API_Vector3(const float& value);

		API_Vector3(const math::float3& other);

		~API_Vector3();

		bool operator==(const API_Vector3& other);

		bool operator!=(const API_Vector3& other);

		API_Vector3 operator= (const API_Vector2& other);

		API_Vector3 operator= (const math::float3& other);

		API_Vector3 operator= (const math::float2& other);

		API_Vector3 operator+(const API_Vector3& other);

		API_Vector3 operator-(const API_Vector3& other);

		API_Vector3 operator+=(const API_Vector3& other);

		API_Vector3 operator-=(const API_Vector3& other);

		API_Vector3 operator*(const float& other);

		API_Vector3 operator/(const float& other);

		API_Vector3 operator*=(const float& other);

		API_Vector3 operator/=(const float& other);

		API_Vector3 operator -() const;

		//friend std::ostream& operator<<(std::ostream& output, const API_Vector3& vec3)
		//{
		//	output << "X : " << vec3.x << "\nY : " << vec3.y << "\nZ : " << vec3.z;
		//	return output;
		//}

		float Distance(const API_Vector3& other);

		static float S_Distance(const API_Vector3& vec, const API_Vector3& vec2);

		static API_Vector3& S_Zero();

		static API_Vector3& S_Identity();

		static API_Vector3& S_Up();

		static API_Vector3& S_Down();

		static API_Vector3& S_Left();

		static API_Vector3& S_Right();

		static API_Vector3& S_Foward();

		static API_Vector3& S_Back();

		float x = 0;

		float y = 0;

		float z = 0;
	};
}
