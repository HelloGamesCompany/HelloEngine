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

//TODO: Every Mesh should have an ID that identifies it's original form, so they can be distributed on diferent render managers.
//TODO: Define what a MeshObject is.
//TODO: Check process of creating the first Mesh inside the Render Manager. Not every Mesh needs the vertices and indices vectors (only the first one does).
class Mesh
{
public:
	Mesh();
	~Mesh();

	void InitAsMeshInformation(float3 position, float3 scale);
	void InitAsCube(float3 position, float3 scale);
	void InitAsSphere(float3 position, float3 scale);

	void Update();

	void CleanUp();

	float3 scale;
	float3 position;

	std::vector<float3>* _vertices;
	std::vector<uint>* _indices;
	mat4x4 modelMatrix;
private:

};

#endif // !__PRIMITIVE_H__