#include "Headers.h"
#include "API_Vector3.h"
#include "API_Vector2.h"

using namespace API;

API_Vector3::API_Vector3()
{
}

API::API_Vector3::API_Vector3(const float& x, const float& y, const float& z) :x(x), y(y), z(z)
{

}

API::API_Vector3::API_Vector3(const float& value) : x(value), y(value), z(value)
{
}

API::API_Vector3::API_Vector3(const float3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

API_Vector3::~API_Vector3()
{
}

bool API_Vector3::operator==(const API_Vector3& other)
{
	if (other.x == this->x && other.y == this->y && other.z == this->z)
		return true;

	return false;
}

bool API_Vector3::operator!=(const API_Vector3& other)
{
	if (other.x == this->x && other.y == this->y && other.z == this->z)
		return false;

	return true;
}

API_Vector3 API_Vector3::operator=(const API_Vector2& other)
{
	API_Vector3 ret;

	ret.x = other.x;
	ret.y = other.y;

	this->x = ret.x;
	this->y = ret.y;

	return ret;
}

API_Vector3 API::API_Vector3::operator=(const math::float3& other)
{
	API_Vector3 ret;

	ret.x = other.x;
	ret.y = other.y;
	ret.z = other.z;

	this->x = ret.x;
	this->y = ret.y;
	this->z = ret.z;

	return ret;
}

API_Vector3 API::API_Vector3::operator=(const math::float2& other)
{
	API_Vector3 ret;

	ret.x = other.x;
	ret.y = other.y;

	this->x = ret.x;
	this->y = ret.y;

	return ret;
}

API_Vector3 API_Vector3::operator+(const API_Vector3& other)
{
	API_Vector3 ret;

	ret.x = this->x + other.x;
	ret.y = this->y + other.y;
	ret.z = this->z + other.z;

	return ret;
}

API_Vector3 API_Vector3::operator-(const API_Vector3& other)
{
	API_Vector3 ret;

	ret.x = this->x - other.x;
	ret.y = this->y - other.y;
	ret.z = this->z - other.z;

	return ret;
}

API_Vector3 API_Vector3::operator+=(const API_Vector3& other)
{
	API_Vector3 ret;

	ret.x = (this->x += other.x);
	ret.y = (this->y += other.y);
	ret.z = (this->z += other.z);

	return ret;
}

API_Vector3 API_Vector3::operator-=(const API_Vector3& other)
{
	API_Vector3 ret;

	ret.x = (this->x -= other.x);
	ret.y = (this->y -= other.y);
	ret.z = (this->z -= other.z);

	return ret;
}

API_Vector3 API_Vector3::operator*(const float& other)
{
	API_Vector3 ret;

	ret.x = this->x * other;
	ret.y = this->y * other;
	ret.z = this->z * other;

	return ret;
}

API_Vector3 API_Vector3::operator/(const float& other)
{
	API_Vector3 ret;

	ret.x = this->x / other;
	ret.y = this->y / other;
	ret.z = this->z / other;

	return ret;
}

API_Vector3 API_Vector3::operator*=(const float& other)
{
	API_Vector3 ret;

	ret.x = (this->x *= other);
	ret.y = (this->y *= other);
	ret.z = (this->z *= other);

	return ret;
}

API_Vector3 API_Vector3::operator/=(const float& other)
{
	API_Vector3 ret;

	ret.x = (this->x /= other);
	ret.y = (this->y /= other);
	ret.z = (this->z /= other);

	return ret;
}

API_Vector3 API::API_Vector3::operator-() const
{
	return API::API_Vector3(-x, -y, -z);
}

float API::API_Vector3::Distance(const API_Vector3& other)
{
	return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2) + pow(this->z - other.z, 2));
}

float API::API_Vector3::S_Distance(const API_Vector3& vec, const API_Vector3& vec2)
{
	return sqrt(pow(vec.x - vec2.x, 2) + pow(vec.y - vec2.y, 2) + pow(vec.z - vec2.z, 2));
}

API_Vector3& API::API_Vector3::S_Zero()
{
	API_Vector3 ret(0, 0, 0);

	return ret;
}

API_Vector3& API::API_Vector3::S_Identity()
{
	API_Vector3 ret(1, 1, 1);

	return ret;
}

API_Vector3& API::API_Vector3::S_Up()
{
	API_Vector3 ret(0, 1, 0);

	return ret;
}

API_Vector3& API::API_Vector3::S_Down()
{
	API_Vector3 ret(0, -1, 0);

	return ret;
}

API_Vector3& API::API_Vector3::S_Left()
{
	API_Vector3 ret(-1, 0, 0);

	return ret;
}

API_Vector3& API::API_Vector3::S_Right()
{
	API_Vector3 ret(1, 0, 0);

	return ret;
}

API_Vector3& API::API_Vector3::S_Foward()
{
	API_Vector3 ret(0, 0, -1);

	return ret;
}

API_Vector3& API::API_Vector3::S_Back()
{
	API_Vector3 ret(0, 0, 1);

	return ret;
}
