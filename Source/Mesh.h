#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "glmath.h"
#include "Color.h"
#include "Math/float3.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void InitAsCube(float3 position, float3 transform);
	void InitAsSphere(float3 position, float3 transform);

	void Update();

	void CleanUp();

	float3 transform;
	float3 position;

	std::vector<float3>* _vertices;
	std::vector<uint>* _indices;
private:
	std::vector<float3>* _originalShape;
};

#endif // !__PRIMITIVE_H__