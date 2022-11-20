#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

#define _USE_MATH_DEFINES

#include <math.h>

Mesh::Mesh()
{
	modelMatrix.SetIdentity();
}

Mesh::~Mesh()
{
	RELEASE(drawPerMeshShader);
	if (_VAO != 0)
	{
		CleanUp();
		glDeleteBuffers(1, &_VAO);
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_IBO);
	}
}

void Mesh::CreateBufferData()
{ 
	// Create Vertex Array Object
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices->size(), &_vertices->front(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Create Index Buffer Object
    glGenBuffers(1, &_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * _indices->size(), &_indices->front(), GL_STATIC_DRAW);

    glBindVertexArray(0);

	drawPerMeshShader = new Shader("Resources/shaders/basic.vertex.shader", "Resources/shaders/basic.fragment.shader");

}

void Mesh::Draw()
{
	drawPerMeshShader->Bind();
	if (textureID != -1) // This should never happen in theory. If a mesh is being drawn with this method, it is because it contains a transparent texture.
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		drawPerMeshShader->SetInt("textureID", textureID);
	}

	drawPerMeshShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	drawPerMeshShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	drawPerMeshShader->SetMatFloat4v("model", &modelMatrix.v[0][0]);

	glBindVertexArray(_VAO);

	glDrawElements(GL_TRIANGLES, _indices->size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

bool Mesh::Update()
{
	if (!draw || outOfFrustum) 
		return false;
	if (isTransparent) // We dont use the TextureManager to set transparent textures.
		return true;
	if (TextureManager::loadedTextures.find(textureID) != TextureManager::loadedTextures.end())
	{
		OpenGLTextureID = TextureManager::BindTexture(textureID);
	}

	return true;
}

void Mesh::InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	this->_vertices = new std::vector<Vertex>(vertices);
	this->_indices = new std::vector<uint>(indices);
}

void Mesh::CleanUp()
{	
	RELEASE(_vertices);
	RELEASE(_indices);
}

void Mesh::CalculateBoundingBoxes()
{
	globalOBB = localAABB;
	globalOBB.Transform(modelMatrix.Transposed());

	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(globalOBB);
}
