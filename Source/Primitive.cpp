#include "Headers.h"
#include "Primitive.h"

// ------------------------------------------------------------
Primitives::Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{
	std::cout << "Primitive" << std::endl;
}

// ------------------------------------------------------------
PrimitiveTypes Primitives::Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitives::Primitive::Render() const
{
	std::cout << "Render primitive" << std::endl;
}

// ------------------------------------------------------------
void Primitives::Primitive::InnerRender() 
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex4f(0.0f, 0.0f, 0.0f,1.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitives::Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitives::Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitives::Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Primitives::Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	GenerateVertexBuffer();
}

Primitives::Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	GenerateVertexBuffer();
}

Primitives::Cube::~Cube()
{
	delete vertices;
	delete indices;
}

void Primitives::Cube::InnerRender() 
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices->front());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), &indices->front());

	glDrawElements(GL_TRIANGLES, indices->size()/*NUM OF INDICES*/, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Primitives::Cube::GenerateVertexBuffer()
{
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	vertices = new std::vector<float3>
	{ 
		{-sx,	 sy,	 sz},
		{-sx,	-sy,	 sz},
		{ sx,	 sy,	 sz},
		{ sx,	-sy,	 sz},
		{-sx,	 sy,	-sz},
		{-sx,	-sy,	-sz},
		{ sx,	 sy,	-sz},
		{ sx,	-sy,	-sz},
	};

	indices = new std::vector<uint>
	{
	  0, 2, 3, 0, 3, 1,
	  2, 6, 7, 2, 7, 3,
	  6, 4, 5, 6, 5, 7,
	  4, 0, 1, 4, 1, 5,
	  0, 4, 6, 0, 6, 2,
	  1, 5, 7, 1, 7, 3,
	};

	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 1000/*NUM OF INDICES*/, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

// SPHERE ============================================
Primitives::Sphere::Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Primitives::Sphere::Sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void Primitives::Sphere::InnerRender() 
{
	// TODO: Implement using glew?
	//glutSolidSphere(radius, 25, 25);
}

// CYLINDER ============================================
Primitives::Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Primitives::Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Primitives::Cylinder::InnerRender() 
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
Primitives::Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Primitives::Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Primitives::Line::InnerRender() 
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Primitives::Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Primitives::Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Primitives::Plane::InnerRender() 
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 2000.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::InitAsCube(float3 position, float3 transform)
{
	this->position = position;
	this->transform = transform;

	_originalShape = new std::vector<float3>
	{
		float3(-0.5,	 0.5,	 0.5),
		float3(-0.5,	-0.5,	 0.5),
		float3( 0.5,	 0.5,	 0.5),
		float3( 0.5,	-0.5,	 0.5),
		float3(-0.5,	 0.5,	-0.5),
		float3(-0.5,	-0.5,	-0.5),
		float3( 0.5,	 0.5,	-0.5),
		float3( 0.5,	-0.5,	-0.5),
	};

	_vertices = new std::vector<float3>(*_originalShape);

	_indices = new std::vector<uint>
	{
	  0, 2, 3, 0, 3, 1,
	  2, 6, 7, 2, 7, 3,
	  6, 4, 5, 6, 5, 7,
	  4, 0, 1, 4, 1, 5,
	  0, 4, 6, 0, 6, 2,
	  1, 5, 7, 1, 7, 3,
	};
}

void Mesh::Update()
{
	for (int i = 0; i < _vertices->size(); i++)
	{
		_vertices->at(i).x = (_originalShape->at(i).x * transform.x) + position.x;
		_vertices->at(i).y = (_originalShape->at(i).y * transform.y) + position.y;
		_vertices->at(i).z = (_originalShape->at(i).z * transform.z) + position.z;
	}
}

void Mesh::CleanUp()
{	
	RELEASE(_vertices);
	RELEASE(_indices);
	RELEASE(_originalShape);
}
