#include "Headers.h"
#include "API_Vector2.h"
#include "API_Vector3.h"

using namespace API;

API_Vector2::API_Vector2()
{
}

API::API_Vector2::API_Vector2(const float& x, const float& y) :x(x), y(y)
{
}

API::API_Vector2::API_Vector2(const float& value) : x(value), y(value)
{
}

API::API_Vector2::API_Vector2(const float2& other)
{
	this->x = other.x;
	this->y = other.y;
}

API_Vector2::~API_Vector2()
{
}

bool API_Vector2::operator==(const API_Vector2& other)
{
	if (other.x == this->x && other.y == this->y)
		return true;

	return false;
}

bool API_Vector2::operator!=(const API_Vector2& other)
{
	if (other.x == this->x && other.y == this->y)
		return false;

	return true;
}

API_Vector2 API_Vector2::operator=(const API_Vector3& other)
{
	API_Vector2 ret;

	ret.x = other.x;
	ret.y = other.y;

	this->x = other.x;
	this->y = other.y;

	return ret;
}

API_Vector2 API::API_Vector2::operator=(const math::float3& other)
{
	API_Vector2 ret;

	ret.x = other.x;
	ret.y = other.y;

	this->x = other.x;
	this->y = other.y;

	return ret;
}

API_Vector2 API_Vector2::operator=(const math::float2& other)
{
	API_Vector2 ret;

	ret.x = other.x;
	ret.y = other.y;

	this->x = other.x;
	this->y = other.y;

	return ret;
}

API_Vector2 API_Vector2::operator+(const API_Vector2& other)
{
	API_Vector2 ret;

	ret.x = this->x + other.x;
	ret.y = this->y + other.y;

	return ret;
}

API_Vector2 API_Vector2::operator-(const API_Vector2& other)
{
	API_Vector2 ret;

	ret.x = this->x - other.x;
	ret.y = this->y - other.y;

	return ret;
}

API_Vector2 API_Vector2::operator+=(const API_Vector2& other)
{
	API_Vector2 ret;

	ret.x = (this->x += other.x);
	ret.y = (this->y += other.y);

	return ret;
}

API_Vector2 API_Vector2::operator-=(const API_Vector2& other)
{
	API_Vector2 ret;

	ret.x = (this->x += other.x);
	ret.y = (this->y += other.y);

	return ret;
}

API_Vector2 API_Vector2::operator*(const float& other)
{
	API_Vector2 ret;

	ret.x = this->x * other;
	ret.y = this->y * other;

	return ret;
}

API_Vector2 API_Vector2::operator/(const float& other)
{
	API_Vector2 ret;

	ret.x = this->x / other;
	ret.y = this->y / other;

	return ret;
}

API_Vector2 API_Vector2::operator*=(const float& other)
{
	API_Vector2 ret;

	ret.x = (this->x *= other);
	ret.y = (this->y *= other);

	return ret;
}

API_Vector2 API_Vector2::operator/=(const float& other)
{
	API_Vector2 ret;

	ret.x = (this->x /= other);
	ret.y = (this->y /= other);

	return ret;
}

API_Vector2 API::API_Vector2::operator-() const
{
	return API::API_Vector2(-x, -y);
}

float API::API_Vector2::Distance(const API_Vector2& other)
{
	return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
}

float API::API_Vector2::S_Distance(const API_Vector2& vec, const API_Vector2& vec2)
{
	return sqrt(pow(vec.x - vec2.x, 2) + pow(vec.y - vec2.y, 2));
}

API_Vector2& API::API_Vector2::S_Zero()
{
	API_Vector2 ret(0, 0);

	return ret;
}

API_Vector2& API::API_Vector2::S_Identity()
{
	API_Vector2 ret(1, 1);

	return ret;
}

API_Vector2& API::API_Vector2::S_Up()
{
	API_Vector2 ret(0, 1);

	return ret;
}

API_Vector2& API::API_Vector2::S_Down()
{
	API_Vector2 ret(0, -1);

	return ret;
}

API_Vector2& API::API_Vector2::S_Left()
{
	API_Vector2 ret(-1, 0);

	return ret;
}

API_Vector2& API::API_Vector2::S_Right()
{
	API_Vector2 ret(1, 0);

	return ret;
}
