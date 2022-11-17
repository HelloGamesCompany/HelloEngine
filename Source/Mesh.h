#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Math/float3.h"

class MeshRenderComponent;

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), texCoords(texC){}
	float3 position = { 0,0,0 };
	float3 normals = { 0,0,0 };
	float2 texCoords = { 0,0 };
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

	void InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices, float3 pos, float3 scale);

	bool Update();

	void CleanUp();

	void SetPosition(float3 pos);
	void SetScale(float3 s);
	void SetRotation(float3 rot);

	void SetTransform(float3 pos, float3 s, float3 rot);

	void CalculateBoundingBoxes();

public:
	float3 rotation;
	float3 scale;
	float3 position;

	std::vector<Vertex>* _vertices = nullptr;
	std::vector<uint>* _indices = nullptr;
	float4x4 modelMatrix = modelMatrix.identity;
	
	float OpenGLTextureID = -1;
	float textureID = -1;

	bool showVertexNormals = false;
	bool showFaceNormals = false;
	bool showAABB = false;
	bool showOBB = false;

	bool draw = true;
	bool outOfFrustum = false;

	OBB globalOBB;
	AABB globalAABB;
	AABB localAABB;

private:
	MeshRenderComponent* component = nullptr;
	bool _updateMatrix = true;

	friend class MeshRenderComponent;
};

#endif // !__PRIMITIVE_H__