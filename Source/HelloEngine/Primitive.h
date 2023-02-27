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
	float3 size;
};

// ============================================
struct PrimSphere : public Primitive
{
public:
	float radius;
};

// ============================================
struct PrimCylinder : public Primitive
{
public:
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