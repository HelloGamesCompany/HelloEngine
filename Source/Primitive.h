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

	void Update();

	void CleanUp();

	float3 transform;
	float3 position;

	std::vector<float3>* _vertices;
	std::vector<uint>* _indices;
private:
	std::vector<float3>* _originalShape;
};

namespace Primitives
{
	class Primitive
	{
	public:
		Primitive();

		virtual void	Render() const;
		virtual void	InnerRender();
		void			SetPos(float x, float y, float z);
		void			SetRotation(float angle, const vec3& u);
		void			Scale(float x, float y, float z);
		PrimitiveTypes	GetType() const;

	public:

		Color color;
		mat4x4 transform;
		bool axis = true, wire = true;

	protected:
		PrimitiveTypes type;
	};

	// ============================================
	//TODO: Make primitives dynamic, so their size and position may change.
	class Cube : public Primitive
	{
	public:
		Cube();
		Cube(float sizeX, float sizeY, float sizeZ);
		~Cube();
		
		void InnerRender() override;

	public:
		vec3 size;
		std::vector<float3>* vertices;
		std::vector<uint>* indices;
	private:
		void GenerateVertexBuffer();
		uint VBO = 0;
		uint IBO = 0;
		uint VAO = 0;
	};

	// ============================================
	class Sphere : public Primitive
	{
	public:
		Sphere();
		Sphere(float radius);
		void InnerRender() override;
	public:
		float radius = 0;
	};

	// ============================================
	class Cylinder : public Primitive
	{
	public:
		Cylinder();
		Cylinder(float radius, float height);
		void InnerRender() override;
	public:
		float radius = 0;
		float height = 0;
	};

	// ============================================
	class Line : public Primitive
	{
	public:
		Line();
		Line(float x, float y, float z);
		void InnerRender() override;
	public:
		vec3 origin;
		vec3 destination;
	};

	// ============================================
	class Plane : public Primitive
	{
	public:
		Plane();
		Plane(float x, float y, float z, float d);
		void InnerRender() override;
	public:
		vec3 normal;
		float constant = 0;
	};

}

#endif // !__PRIMITIVE_H__