#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "InstanceRenderer.h"
#include "ModuleRenderer3D.h"

#include "SkinnedMeshRenderComponent.h"
#include "AnimationComponent.h"

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
	RELEASE(boneMeshShader);
	if (_VAO != 0)
	{
		CleanUp();
	}
	if (Application::Instance()->renderer3D->renderManager._selectedMesh == this)
	{
		Application::Instance()->renderer3D->renderManager.SetSelectedMesh(nullptr);
	}
}

void Mesh::CreateBufferData()
{ 
	_VAO = resource->VAO;
	_VBO = resource->VBO;
	_IBO = resource->IBO;

	drawPerMeshShader = new Shader("Resources/shaders/basic.vertex.shader", "Resources/shaders/basic.fragment.shader");
	boneMeshShader = new Shader("Resources/shaders/basicBone.vertex.shader", "Resources/shaders/basicBone.fragment.shader");
}

void Mesh::Draw(bool useBasicShader)
{
	if (useBasicShader) // We use this function to draw the outilne too.
	{
		if (textureID != -1) // Only happens when material components is deactivated.
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		if (component->_hasBones)
		{
			boneMeshShader->Bind();
			boneMeshShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
			boneMeshShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
			boneMeshShader->SetMatFloat4v("model", &modelMatrix.v[0][0]);

			SkinnedMeshRenderComponent* smComp = (SkinnedMeshRenderComponent*) component;

			if (!smComp->hasAnim)
			{
				smComp->UpdateBones();
			}

			for (int i = 0; i < smComp->goBonesArr.size(); ++i)
			{
				boneMeshShader->SetMatFloat4v("finalBonesMatrices[" + std::to_string(i) + "]", &smComp->goBonesArr[i].Transposed().v[0][0]);
			}

		}
		else
		{
			drawPerMeshShader->Bind();
			drawPerMeshShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
			drawPerMeshShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
			drawPerMeshShader->SetMatFloat4v("model", &modelMatrix.v[0][0]);
		}
		
	}

	glBindVertexArray(_VAO);

	glDrawElements(GL_TRIANGLES, _indices->size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Mesh::Update()
{
	if (!draw)
		return false;

	if (showVertexNormals)
		Application::Instance()->renderer3D->renderManager.DrawVertexNormals(this);
	if (showFaceNormals)
		Application::Instance()->renderer3D->renderManager.DrawFaceNormals(this);
	if (showAABB)
		Application::Instance()->renderer3D->renderManager.DrawAABB(this);
	if (showOBB)
		Application::Instance()->renderer3D->renderManager.DrawOBB(this);

	if (outOfFrustum) 
		return false;
	if (component && component->_gameObject->isSelected)
	{
		Application::Instance()->renderer3D->renderManager.SetSelectedMesh(this);
		return false; // We dont want to render this object twice when selected.
	}
	if (isIndependent) // We dont use the TextureManager to set independent meshes's textures.
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
	if (isIndependent)
		Draw();
	else
	{
		InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID);
		manager->DrawInstance(this);
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

#ifdef STANDALONE
	stencilShader.Bind();
	stencilShader.SetFloat("outlineSize", 0.04f);
	stencilShader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	stencilShader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	stencilShader.SetMatFloat4v("model", &modelMatrix.v[0][0]);
#endif
	// Draw model bigger size using the stencilShader
	if (isIndependent)
		Draw(false);
	else
	{
		InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID);
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
	resource = res;
	this->_vertices = &res->meshInfo.vertices;
	this->_indices = &res->meshInfo.indices;
}

void Mesh::CleanUp()
{	
	//RELEASE(_vertices);
	//RELEASE(_indices);
}

void Mesh::CalculateBoundingBoxes()
{
	globalOBB = localAABB;
	globalOBB.Transform(modelMatrix.Transposed());

	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(globalOBB);
}
