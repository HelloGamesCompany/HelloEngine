#pragma once

namespace API
{
	class API_Vector3;

	class API_Vector2
	{
	public:
		API_Vector2();

		API_Vector2(const float& x, const float& y);

		API_Vector2(const float& value);

		~API_Vector2();

		bool operator==(const API_Vector2& other);

		bool operator!=(const API_Vector2& other);

		API_Vector2& operator= (const API_Vector3& other);

		API_Vector2& operator= (const math::float3& other);

		API_Vector2& operator= (const math::float2& other);

		API_Vector2& operator+(const API_Vector2& other);

		API_Vector2& operator-(const API_Vector2& other);

		API_Vector2& operator+=(const API_Vector2& other);

		API_Vector2& operator-=(const API_Vector2& other);

		API_Vector2& operator*(const float& other);

		API_Vector2& operator/(const float& other);

		API_Vector2& operator*=(const float& other);

		API_Vector2& operator/=(const float& other);

		friend std::ostream& operator<<(std::ostream& output, const API_Vector2& vec2)
		{
			output << "X : " << vec2.x << "\nY : " << vec2.y;
			return output;
		}

		float Distance(const API_Vector2& other);

		static float S_Distance(const API_Vector2& vec, const API_Vector2& vec2);

		// DotProduct

		static API_Vector2& S_Zero();

		static API_Vector2& S_Identity();

		static API_Vector2& S_Up();

		static API_Vector2& S_Down();

		static API_Vector2& S_Left();

		static API_Vector2& S_Right();

		float x = 1;

		float y = 1;
	};
}