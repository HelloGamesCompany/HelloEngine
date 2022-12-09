#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Math/float3.h"
#include "Shader.h"


class MeshRenderComponent;
class ResourceMesh;

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), texCoords(texC){}
	float3 position = { 0,0,0 };
	float3 normals = { 0,0,0 };
	float2 texCoords = { 0,0 };
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Only to be used for meshes that cannot be drawn with instanced rendering (meshes with transparency).
	void CreateBufferData();
	void Draw(bool useBasicShader = true);
	// ----------------------------------------------------------------------------------------------------

	void DrawAsSelected();

	void InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);
	void InitWithResource(ResourceMesh* res);

	bool Update();

	void CleanUp();

	void CalculateBoundingBoxes();

public:

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

	bool isTransparent = false;

private:
	MeshRenderComponent* component = nullptr;
	bool _updateMatrix = true;

	// Only to be used for meshes that cannot be drawn with instanced rendering (meshes with transparency).
	uint _VAO = 0;
	uint _VBO = 0;
	uint _IBO = 0;
	Shader* drawPerMeshShader = nullptr;
	// ----------------------------------------------------------------------------------------------------

#ifdef STANDALONE
	Shader stencilShader;
#endif

	friend class MeshRenderComponent;
	friend class InstanceRenderer;
};

#endif // !__PRIMITIVE_H__