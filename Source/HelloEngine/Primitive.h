#pragma once

#include "MathGeoLib.h"

enum class PrimitiveTypes
{
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

struct Primitive
{
public:
	PrimitiveTypes	GetType() const
	{
		return type;
	};

public:
	float4x4 transform;

protected:
	PrimitiveTypes type;
};

// ============================================
struct PrimCube : public Primitive
{
public:
	PrimCube(float size = 1)
	{
		this->size.x = size;
		this->size.y = size;
		this->size.z = size;
		type = PrimitiveTypes::Primitive_Cube;
	}
	float3 size;
};

// ============================================
struct PrimSphere : public Primitive
{
public:
	PrimSphere(float radius = 1) 
	{
		this->radius = radius; 
		type = PrimitiveTypes::Primitive_Sphere;
	}
	float radius;
};

// ============================================
struct PrimCylinder : public Primitive
{
public:
	PrimCylinder(float radius = 0.5f, float height = 1) 
	{
		this->radius = radius;
		this->height = height;
		type = PrimitiveTypes::Primitive_Cylinder;
	}
	float radius;
	float height;
};

// ============================================
struct PrimLine : public Primitive
{
public:
	float3 origin;
	float3 destination;
};

// ============================================
struct PrimPlane : public Primitive
{
public:
	float3 normal;
	float constant;
};