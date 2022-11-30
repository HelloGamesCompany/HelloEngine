#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "RenderManager.h"
#include "ModuleRenderer3D.h"

#define _USE_MATH_DEFINES

#include <math.h>

Mesh::Mesh()
{
	modelMatrix.SetIdentity();
#ifdef STANDALONE
	stencilShader = Shader("Resources/shaders/stencil.vertex.shader", "Resources/shaders/stencil.fragment.shader");
#endif

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

void Mesh::Draw(bool useBasicShader)
{
	if (useBasicShader) // We use this function to draw the outilne too.
	{
		if (textureID != -1) // Only happens when material components is deactivated.
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
		drawPerMeshShader->Bind();
		drawPerMeshShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
		drawPerMeshShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
		drawPerMeshShader->SetMatFloat4v("model", &modelMatrix.v[0][0]);
	}

	glBindVertexArray(_VAO);

	glDrawElements(GL_TRIANGLES, _indices->size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Mesh::Update()
{
	if (!draw || outOfFrustum) 
		return false;
	if (component && component->_gameObject->isSelected)
	{
		DrawAsSelected();
		return false; // We dont want to render this object twice when selected.
	}
	if (isTransparent) // We dont use the TextureManager to set transparent textures.
		return true;
	
	if (textureID != -1.0f)
	{
		OpenGLTextureID = TextureManager::BindTexture(textureID);
	}

	return true;
}

void Mesh::DrawAsSelected()
{
	//// TODO: Do this inside ModuleRender3D, and allow to enable and disable it.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	// Draw model normal size
	if (isTransparent)
		Draw();
	else
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(component->_meshID);
		manager->DrawInstance(this);
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	stencilShader.Bind();
	stencilShader.SetFloat("outlineSize", 0.04f);
	stencilShader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	stencilShader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	stencilShader.SetMatFloat4v("model", &modelMatrix.v[0][0]);

	// Draw model bigger size using the stencilShader
	if (isTransparent)
		Draw(false);
	else
	{
		RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(component->_meshID);
		manager->DrawInstance(this, false);
	}

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Mesh::InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	this->_vertices = new std::vector<Vertex>(vertices);
	this->_indices = new std::vector<uint>(indices);
}

void Mesh::InitWithResource(ResourceMesh* res)
{
	this->_vertices = &res->meshInfo.vertices;
	this->_indices = &res->meshInfo.indices;
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
