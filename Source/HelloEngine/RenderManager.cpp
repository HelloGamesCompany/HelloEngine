#include "Headers.h"
#include "ModulePhysics.h"
#include "RenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "MeshImporter.h"
#include "CameraObject.h"
#include "ModuleCamera3D.h"
#include "ComponentUIButton.h"
#include "ComponentUISlider.h"
#include "ComponentUICheckbox.h"
#include "ComponentUIImage.h"
#include "Math/float4x4.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
	RELEASE(_textureManager);
	RELEASE(lineShader);
	RELEASE(localLineShader);
}

void RenderManager::Init()
{
	_textureManager = new TextureManager();

	cubeUID = 2523359550;
	planeUID = 90291865;
	cylinderUID = 2734209052;
	sphereUID = 4243659739;
	plane2DUID = 90291866;

	// Create resources for Primitives
	ResourceMesh* cubeResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2523359550.hmesh", cubeUID, "Cube", false);
	ResourceMesh* planeResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/90291865.hmesh", planeUID, "Plane", false);
	ResourceMesh* plane2DResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/90291865.hmesh", plane2DUID, "Plane2D", false);
	ResourceMesh* cylinderResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2734209052.hmesh", cylinderUID, "Cylinder", false);
	ResourceMesh* sphereResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/4243659739.hmesh", sphereUID, "Sphere", false);

	for (int i = 0; i < 5; ++i)
	{
		primitiveModels[i] = new ResourceModel();
	}

	// Create a Resource Model for every primitive. This is mandatory so Serialization can funciton properly.

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::CUBE] = primitiveModels[(int)PrimitiveType::CUBE];
	cubeResource->modelUID = (uint)PrimitiveModelsUID::CUBE;
	primitiveModels[(int)PrimitiveType::CUBE]->modelMeshes.push_back(cubeResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::CYLINDER] = primitiveModels[(int)PrimitiveType::CYLINDER];
	cylinderResource->modelUID = (uint)PrimitiveModelsUID::CYLINDER;
	primitiveModels[(int)PrimitiveType::CYLINDER]->modelMeshes.push_back(cylinderResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::PLANE] = primitiveModels[(int)PrimitiveType::PLANE];
	planeResource->modelUID = (uint)PrimitiveModelsUID::PLANE;
	primitiveModels[(int)PrimitiveType::PLANE]->modelMeshes.push_back(planeResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::PLANE2D] = primitiveModels[(int)PrimitiveType::PLANE2D];
	plane2DResource->modelUID = (uint)PrimitiveModelsUID::PLANE2D;
	primitiveModels[(int)PrimitiveType::PLANE2D]->modelMeshes.push_back(plane2DResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::SPHERE] = primitiveModels[(int)PrimitiveType::SPHERE];
	sphereResource->modelUID = (uint)PrimitiveModelsUID::SPHERE;
	primitiveModels[(int)PrimitiveType::SPHERE]->modelMeshes.push_back(sphereResource);

	// Init shaders
	lineShader = new Shader("Resources/shaders/lines.vertex.shader", "Resources/shaders/lines.fragment.shader");
	localLineShader = new Shader("Resources/shaders/localLines.vertex.shader", "Resources/shaders/localLines.fragment.shader");

	// Set up debug drawing variables:
	// Manually created box index buffer that corresponds to the order given by MathGeoLib's AABB class GetCornerPoints() method.
	boxIndices.push_back(0);    // 1
	boxIndices.push_back(1);    // 2
	boxIndices.push_back(0);    // 3
	boxIndices.push_back(2);    // 4
	boxIndices.push_back(2);    // 5
	boxIndices.push_back(3);    // 6
	boxIndices.push_back(1);    // 7
	boxIndices.push_back(3);    // 8
	boxIndices.push_back(0);    // 9
	boxIndices.push_back(4);    // 10
	boxIndices.push_back(4);    // 11
	boxIndices.push_back(5);    // 12
	boxIndices.push_back(4);    // 13
	boxIndices.push_back(6);    // 14
	boxIndices.push_back(6);    // 15
	boxIndices.push_back(7);    // 16
	boxIndices.push_back(7);    // 17
	boxIndices.push_back(5);    // 18
	boxIndices.push_back(1);    // 19
	boxIndices.push_back(5);    // 20
	boxIndices.push_back(3);    // 21
	boxIndices.push_back(7);    // 22
	boxIndices.push_back(2);    // 23
	boxIndices.push_back(6);    // 24

	 // Set up buffer for OBB lines.
	glGenVertexArrays(1, &OBBVAO);
	glBindVertexArray(OBBVAO);

	glGenBuffers(1, &OBBIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OBBIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &OBBVBO);
	glBindBuffer(GL_ARRAY_BUFFER, OBBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	// Set up buffer for AABB lines.
	glGenVertexArrays(1, &AABBVAO);
	glBindVertexArray(AABBVAO);

	glGenBuffers(1, &AABBIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &AABBVBO);
	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	CalculateSphereBuffer();
	CalculateCylinderBuffer();

	
	
	
}

void RenderManager::OnEditor()
{
	if (ImGui::Begin("Render System Debugging"))
	{
		ImGui::TextWrapped("Window to manage every RenderManager memory usage and instances. The memory allocated per RenderManager increases by 50 per cent every time the current allocated memory gets surpassed.");
		for (auto& manager : _renderMap)
		{
			if (manager.second.resource == nullptr)
				continue;
			std::string headerName = manager.second.resource->debugName + "##" + std::to_string(manager.second.resource->UID);
			if(ImGui::CollapsingHeader(headerName.c_str()))
			{
				std::string maxInstances = "Maximum number of instances: " + std::to_string(manager.second.instanceNum);
				ImGui::Text(maxInstances.c_str());
				ImGui::Text("Instances: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(manager.second.meshes.size()).c_str());
				ImGui::Text("Memory usage in bytes: "); ImGui::SameLine();
				float memory = manager.second.instanceNum * sizeof(float4x4) + manager.second.instanceNum * sizeof(float);
				memory += manager.second.totalIndices->size() * sizeof(uint);
				memory += manager.second.totalVertices->size() * sizeof(Vertex);
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), std::to_string(memory).c_str());
			}
		}
	}
	ImGui::End();
}

InstanceRenderer* RenderManager::GetRenderManager(uint ID, bool create)
{
	// If there is no instance Renderer for this mesh resource
	if (_renderMap.count(ID) == 0)
	{
		if (create)
			_renderMap[ID].SetMeshInformation((ResourceMesh*)ModuleResourceManager::resources[ID]);
		else
			return nullptr;
	}

	return &_renderMap[ID];
}

void RenderManager::Draw()
{
	// Draw opaque meshes instanced.
	for (auto& obj : _renderMap)
	{
		obj.second.Draw();
	}
	// Delete empty render managers.
	for (int i = 0; i < _emptyRenderManagers.size(); i++)
	{
		_renderMap.erase(_emptyRenderManagers[i]);
	}
	_emptyRenderManagers.clear();
	
	// Draw meshes that must be rendered in an individual draw call.
	DrawIndependentMeshes();
	// Draw selected mesh
	DrawSelectedMesh();

	// Draw meshes that have transparency textures applied on their material.
	DrawTransparentMeshes();

#ifdef STANDALONE
	for (int i = 0; i < ModulePhysics::physBodies.size(); i++) 
	{
		ModulePhysics::physBodies[i]->RenderCollider();
	}
#endif // STANDALONE


}

void RenderManager::Draw2D()
{
	// Draw all 2D meshes.
	if (renderer2D != nullptr)
		renderer2D->Draw2D();
}

uint RenderManager::AddMesh(ResourceMesh* resource, MeshRenderType type)
{
	switch (type)
	{
	case MeshRenderType::INSTANCED:
		return AddInstancedMesh(resource);
	case MeshRenderType::INDEPENDENT:
		return AddIndependentMesh(resource);
	case MeshRenderType::TRANSPARENCY:
		return AddTransparentMesh(resource);
	case MeshRenderType::MESH2D:
		return Add2DMesh();
	}
}

uint RenderManager::AddTransparentMesh(ResourceMesh* resource)
{
	uint randomID = HelloUUID::GenerateUUID();

	_transparencyMeshes[randomID].InitWithResource(resource);
	_transparencyMeshes[randomID].localAABB = resource->localAABB;
	_transparencyMeshes[randomID].isIndependent = true;
	_transparencyMeshes[randomID].CreateBufferData();

	return randomID;
}

uint RenderManager::AddIndependentMesh(ResourceMesh* resource)
{
	uint randomID = HelloUUID::GenerateUUID();

	_independentMeshes[randomID].InitWithResource(resource);
	_independentMeshes[randomID].localAABB = resource->localAABB;
	_independentMeshes[randomID].isIndependent = true;
	_independentMeshes[randomID].CreateBufferData();

	return randomID;
}

uint RenderManager::AddInstancedMesh(ResourceMesh* resource)
{
	InstanceRenderer* manager = GetRenderManager(resource->UID); // Create a renderManager.
	return manager->AddMesh();
}

uint RenderManager::Add2DMesh()
{
	InstanceRenderer* manager = GetRenderManager(plane2DUID); // Create a renderManager.
	renderer2D = manager;
	renderer2D->SetAs2D();
	return manager->AddMesh();
}

void RenderManager::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr) 
		parent = ModuleLayers::rootGameObject;

	switch (type)
	{
		case PrimitiveType::CUBE:
		{
			GameObject* cube = new GameObject(parent, "Cube", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(cubeUID);
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* cube = new GameObject(parent, "Sphere", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(sphereUID);
			break;
		}
		case PrimitiveType::CYLINDER:
		{
			GameObject* cube = new GameObject(parent, "Cylinder", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(cylinderUID);
			break;
		}
		case PrimitiveType::PLANE:
		{
			GameObject* cube = new GameObject(parent, "Plane", "Primitive");
			MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			meshRenderer->CreateMesh(planeUID);
			break;
		}
		case PrimitiveType::PLANE2D:
		{
			GameObject* canvas = new GameObject(parent, "Canvas", "UI");

			//GameObject* cube = new GameObject(parent, "Canvas", "Primitive");
			//MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
			//meshRenderer->CreateMesh(plane2DUID);
			break;
		}
	}
}

void RenderManager::CreateUI(GameObject* parent, UIType type)
{
	if (parent == nullptr)
		parent = ModuleLayers::rootGameObject;

	switch (type)
	{
		case UIType::BUTTON:
		{
			GameObject* button = new GameObject(parent, "Button", "UI");
			button->AddComponent<ComponentUIButton>();
			break;
		}
		case UIType::SLIDER:
		{
			GameObject* slider = new GameObject(parent, "Slider", "UI");
			GameObject* sliderButton = new GameObject(slider, "SliderButton", "UIsliderButton");
			GameObject* sliderBar = new GameObject(slider, "SliderBar", "UIsliderBar");
			//slider->AddComponent<ComponentUISlider>();
			sliderBar->AddComponent<ComponentUISlider>();
			sliderButton->AddComponent<ComponentUISlider>();
			break;
		}
		case UIType::CHECKBOX:
		{
			GameObject* checkBox = new GameObject(parent, "CheckBox", "UI");
			checkBox->AddComponent<ComponentUICheckbox>();
			break;
		}
		case UIType::IMAGE:
		{
			GameObject* checkBox = new GameObject(parent, "Image", "UI");
			checkBox->AddComponent<ComponentUIImage>();
			break;
		}
	}
}

void RenderManager::DestroyRenderManager(uint managerUID)
{
	_emptyRenderManagers.push_back(managerUID);
	// If we are destroying the 2D renderer, set its pointer to nullptr.
	if (_renderMap[managerUID].is2D)
		renderer2D = nullptr;
}

void RenderManager::SetSelectedMesh(Mesh* mesh)
{
	_selectedMesh = mesh;
}

void RenderManager::DrawSelectedMesh()
{
	if (_selectedMesh == nullptr)
		return;

	_selectedMesh->DrawAsSelected();

	_selectedMesh = nullptr;
}

void RenderManager::DrawVertexNormals(Mesh* mesh)
{
	lineShader->Bind();
	lineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	lineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	lineShader->SetFloat4("lineColor", 0.36f, 0.75f, 0.72f, 1.0f);

	lineShader->SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);

	glBindVertexArray(mesh->resource->VertexNormalsVAO);

	glDrawArrays(GL_LINES, 0, mesh->resource->vertexNormals.size());

	glBindVertexArray(0);
}

void RenderManager::DrawFaceNormals(Mesh* mesh)
{
	lineShader->Bind();
	lineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	lineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	lineShader->SetFloat4("lineColor", 0.75f, 0.36f, 0.32f, 1.0f);

	lineShader->SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);

	glBindVertexArray(mesh->resource->FaceNormalsVAO);

	glDrawArrays(GL_LINES, 0, mesh->resource->faceNormals.size());

	glBindVertexArray(0);
}

void RenderManager::DrawOBB(Mesh* mesh)
{
	float3 OBBPoints[8];

	mesh->globalOBB.GetCornerPoints(OBBPoints);

	glBindVertexArray(OBBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, OBBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &OBBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->Bind();
	localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->SetFloat4("lineColor", 1.0f, 0.0f, 0.0f, 1.0f);

	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderManager::DrawAABB(Mesh* mesh)
{
	float3 AABBPoints[8];

	mesh->globalAABB.GetCornerPoints(AABBPoints);

	glBindVertexArray(AABBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &AABBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->Bind();
	localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->SetFloat4("lineColor", 0.0f, 1.0f, 0.0f, 1.0f);

	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderManager::DrawColliderBox(PhysBody3D* physBody, float4 color, float wireSize)
{
	float3 AABBPoints[8];

	float mult = WORLD_TO_RENDER_RELATION;

	btBoxShape* shape = (btBoxShape*)physBody->body->getCollisionShape();
	btTransform transform = physBody->body->getWorldTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 origin = transform.getOrigin();

	btVector3 rotationAxis = rotation.getAxis();
	btScalar rotationAngle = rotation.getAngle();

	btVector3 eulerRotation;
	btMatrix3x3 rotationMatrix(rotation);
	rotationMatrix.getEulerZYX(eulerRotation[0], eulerRotation[1], eulerRotation[2]);

	btVector3 aabbMin, aabbMax;
	shape->getAabb(transform, aabbMin, aabbMax);

	float minX = aabbMin.getX();
	float minY = aabbMin.getY();
	float minZ = aabbMin.getZ();
	float maxX = aabbMax.getX();
	float maxY = aabbMax.getY();
	float maxZ = aabbMax.getZ();

	btVector3 corners[8];

	corners[0] = btVector3(minX, minY, minZ);
	corners[1] = btVector3(minX, minY, maxZ);
	corners[2] = btVector3(minX, maxY, minZ);
	corners[3] = btVector3(minX, maxY, maxZ);
	corners[4] = btVector3(maxX, minY, minZ);
	corners[5] = btVector3(maxX, minY, maxZ);
	corners[6] = btVector3(maxX, maxY, minZ);
	corners[7] = btVector3(maxX, maxY, maxZ);

	for (int i = 0; i < 8; i++) {
		shape->getVertex(i, corners[i]);
		corners[i] = transform * corners[i];
	}

	for (int i = 0; i < 8; i++) {
		AABBPoints[i] = { corners[i][0], corners[i][1],corners[i][2] };
	}

	//AABBPoints[0].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[0].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[0].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[1].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[1].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[1].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[2].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[2].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[2].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[3].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[3].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[3].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//
	//AABBPoints[4].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[4].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[4].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[5].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[5].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[5].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[6].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[6].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[6].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	//AABBPoints[7].x = (float)physBody->body->getCenterOfMassTransform().getOrigin().getX() * mult;
	//AABBPoints[7].y = (float)physBody->body->getCenterOfMassTransform().getOrigin().getY() * mult;
	//AABBPoints[7].z = (float)physBody->body->getCenterOfMassTransform().getOrigin().getZ() * mult;

	glBindVertexArray(AABBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &AABBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->Bind();
	localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

	localLineShader->SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);
	
}

void RenderManager::DrawColliderSphere(PhysBody3D* physBody, float radius, float4 color, float wireSize, uint verSlices, uint horSlices)
{
	const float3 origin = (float3)physBody->body->getCenterOfMassTransform().getOrigin();
	const float3 startingPointY = origin - float3(0, radius, 0);
	const float diferenceBetweenSlicesY = (radius * 2) / (horSlices + 1);

	std::vector<float3> SpherePoints;

	SpherePoints.push_back(startingPointY);

	for (int i = 1; i < horSlices + 1; i++)
	{
		for (int j = 0; j < verSlices; j++)
		{
			float tempY = startingPointY.y + diferenceBetweenSlicesY * i;
			float tempYRad = sqrt(Pow(radius, 2) - Pow(-radius + diferenceBetweenSlicesY * i,2));
			float tempX = origin.x + tempYRad * cos(2 * math::pi * j / verSlices);
			float tempZ = origin.z + tempYRad * sin(2 * math::pi * j / verSlices);

			float3 tempPoint = float3(tempX, tempY, tempZ);
			SpherePoints.push_back(tempPoint);
		}
		
	}

	SpherePoints.push_back(origin + float3(0, radius, 0));

	glBindVertexArray(SPVAO);

	glBindBuffer(GL_ARRAY_BUFFER, SPVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &SpherePoints.at(0), SpherePoints.size() * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->Bind();
	localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, sphereIndices.size() , GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);
	
}

void RenderManager::DrawColliderCylinder(PhysBody3D* physBody, float2 radiusHeight, float4 color, float wireSize, uint verSlices)
{
	const float radius = radiusHeight.x;
	const float height = radiusHeight.y;

	const float3 origin = (float3)physBody->body->getCenterOfMassTransform().getOrigin();
	const float3 startingPointY = origin - float3(0, -(height / 2), 0);
	const float3 endingPointY = origin - float3(0, +(height / 2), 0);


	const btQuaternion rotBtQuat = physBody->body->getCenterOfMassTransform().getRotation();

	float3 rotAxis = (float3)rotBtQuat.getAxis();
	float rotAngle = (float)rotBtQuat.getAngle();
	Quat rotQuat = Quat::RotateAxisAngle(rotAxis, DegToRad(rotAngle));



	

	/*float rad = rotQuat.Angle();
	btScalar rotBtAngle = RadToDeg(rad);*/
	
	//float rotAngle = (float)rotBtAngle;

	//float3 rotAxis;

	//rotAxis.x = rotQuat.Axis().x;
	//rotAxis.y = rotQuat.Axis().y;
	//rotAxis.z = rotQuat.Axis().z;

	////SCLrotAxis.Normalized();

	//float rotMatrix[16];

	
		
	/*rotMatrix[0] = (cos(rotAngle) + pow(rotAxis.x, 2) * (1 - cos(rotAngle)));
	rotMatrix[4] = rotAxis.x * rotAxis.y * (1 - cos(rotAngle)) - rotAxis.z * sin(rotAngle);
	rotMatrix[8] = rotAxis.x * rotAxis.z * (1 - cos(rotAngle)) + rotAxis.y * sin(rotAngle);
	rotMatrix[12] = 0;

	rotMatrix[1] = rotAxis.y * rotAxis.x * (1 - cos(rotAngle)) + rotAxis.z * sin(rotAngle);
	rotMatrix[5] = cos(rotAngle) + pow(rotAxis.y, 2) * (1 - cos(rotAngle));
	rotMatrix[9] = rotAxis.y * rotAxis.z * (1 - cos(rotAngle)) - rotAxis.x * sin(rotAngle);
	rotMatrix[13] = 0;

	rotMatrix[2] = rotAxis.z * rotAxis.x * (1 - cos(rotAngle)) - rotAxis.y * sin(rotAngle);
	rotMatrix[6] = rotAxis.z * rotAxis.y * (1 - cos(rotAngle)) + rotAxis.x * sin(rotAngle);
	rotMatrix[10] = cos(rotAngle) + pow(rotAxis.z, 2) * (1 - cos(rotAngle));
	rotMatrix[14] = 0;

	rotMatrix[3] = 0;
	rotMatrix[7] = 0;
	rotMatrix[11] = 0;
	rotMatrix[15] = 1;*/

	//rotMatrix[0] = (cos(rotAngle) + pow(rotAxis.x, 2) * (1 - cos(rotAngle)));
	//rotMatrix[3] = rotAxis.x * rotAxis.y * (1 - cos(rotAngle)) - rotAxis.z * sin(rotAngle);
	//rotMatrix[6] = rotAxis.x * rotAxis.z * (1 - cos(rotAngle)) + rotAxis.y * sin(rotAngle);
	////rotMatrix[9] = 0;

	//rotMatrix[1] = rotAxis.y * rotAxis.x * (1 - cos(rotAngle)) + rotAxis.z * sin(rotAngle);
	//rotMatrix[4] = cos(rotAngle) + pow(rotAxis.y, 2) * (1 - cos(rotAngle));
	//rotMatrix[7] = rotAxis.y * rotAxis.z * (1 - cos(rotAngle)) - rotAxis.x * sin(rotAngle);
	////rotMatrix[10] = 0;

	//rotMatrix[2] = rotAxis.z * rotAxis.x * (1 - cos(rotAngle)) - rotAxis.y * sin(rotAngle);
	//rotMatrix[5] = rotAxis.z * rotAxis.y * (1 - cos(rotAngle)) + rotAxis.x * sin(rotAngle);
	//rotMatrix[8] = cos(rotAngle) + pow(rotAxis.z, 2) * (1 - cos(rotAngle));
	//rotMatrix[11] = 0;

	//rotMatrix[3] = 0;
	//rotMatrix[7] = 0;
	//rotMatrix[11] = 0;
	//rotMatrix[15] = 1;





	
	std::vector<float3> CylinderPoints;
	
	//Down
	for (int i = 0; i < verSlices; i++)
	{
		float tempY = startingPointY.y;
		float tempX = origin.x + radius * cos(2 * math::pi * i / verSlices);
		float tempZ = origin.z + radius * sin(2 * math::pi * i / verSlices);

		float3 tempPoint = float3(tempX, tempY, tempZ);
		float3 rotatedVec = rotQuat * tempPoint;

		CylinderPoints.push_back(rotatedVec);
	}

	//Up
	for (int i = 0; i < verSlices; i++)
	{
		float tempY = endingPointY.y;
		float tempX = origin.x + radius * cos(2 * math::pi * i / verSlices);
		float tempZ = origin.z + radius * sin(2 * math::pi * i / verSlices);

		float3 tempPoint = float3(tempX, tempY, tempZ);
		float3 rotatedVec = rotQuat * tempPoint;
		CylinderPoints.push_back(rotatedVec);
	}

	glBindVertexArray(CYVAO);

	glBindBuffer(GL_ARRAY_BUFFER, CYVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &CylinderPoints.at(0), CylinderPoints.size() * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->Bind();
	localLineShader->SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);
}

void RenderManager::CalculateSphereBuffer(uint verSlices, uint horSlices)
{
	// Set up buffer for Sphere lines.

	sphereIndices.clear();

	for (int j = 1; j < verSlices + 1; j++) {
		sphereIndices.push_back(0);
		sphereIndices.push_back(j);
	}

	for (int i = 0; i < horSlices - 1; i++) {

		//Vertical
		for (int j = 0; j < verSlices; j++) {
			sphereIndices.push_back((i * verSlices + 1) + j);
			sphereIndices.push_back(((i + 1) * verSlices + 1) + j);
		}

		//Horizontal
		for (int j = 0; j < verSlices; j++) {
			if (j == verSlices - 1) {
				sphereIndices.push_back((i * verSlices + 1) + j);
				sphereIndices.push_back((i * verSlices + 1));
			}
			else {
				sphereIndices.push_back((i * verSlices + 1) + j);
				sphereIndices.push_back((i * verSlices + 1) + j + 1);
			}

		}
	}

	for (int i = 0; i < verSlices ; i++) {

		//Vertical
		sphereIndices.push_back(((horSlices - 1 ) * verSlices + 1) + i);
		sphereIndices.push_back(horSlices * verSlices + 1);

		//Horizontal
		if (i == verSlices - 1) {
			sphereIndices.push_back(((horSlices-1) * verSlices + 1) + i);
			sphereIndices.push_back((horSlices - 1) * verSlices  + 1);
		}
		else {
			sphereIndices.push_back(((horSlices - 1) * verSlices + 1) + i);
			sphereIndices.push_back(((horSlices - 1) * verSlices + 1) + i + 1);
		}


	}

	glGenVertexArrays(1, &SPVAO);
	glBindVertexArray(SPVAO);

	glGenBuffers(1, &SPIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SPIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &SPVBO);
	glBindBuffer(GL_ARRAY_BUFFER, SPVBO);
	const uint sphereVertexNum = verSlices * horSlices + 2;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * sphereVertexNum, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void RenderManager::CalculateCylinderBuffer(uint verSlices)
{

	cylinderIndices.clear();

	//Vertical
	for (int i = 0; i < verSlices; i++) {
		cylinderIndices.push_back(i);
		cylinderIndices.push_back(i + verSlices);
	}

	//Horizontal
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < verSlices; j++) {
			if (j == verSlices - 1) {
				cylinderIndices.push_back(i * verSlices + j);
				cylinderIndices.push_back(i * verSlices);
			}
			else {
				cylinderIndices.push_back(i * verSlices + j);
				cylinderIndices.push_back(i * verSlices + j + 1);
			}

		}
	}


	// Set up buffer for Cylinder lines.
	glGenVertexArrays(1, &CYVAO);
	glBindVertexArray(CYVAO);

	glGenBuffers(1, &CYIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CYIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * cylinderIndices.size(), &cylinderIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &CYVBO);
	glBindBuffer(GL_ARRAY_BUFFER, CYVBO);
	const uint cylinderVertexNum = verSlices * 2;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * cylinderVertexNum, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void RenderManager::DestroyInstanceRenderers()
{
	_renderMap.clear();
	renderer2D = nullptr;
}

void RenderManager::DrawTransparentMeshes()
{
	if (_transparencyMeshes.empty())
		return;

	CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;

	// Draw transparent objects with a draw call per mesh.
	for (auto& mesh : _transparencyMeshes)
	{
		float3 cameraPos = currentCamera->cameraFrustum.pos;
		float distance = mesh.second.modelMatrix.Transposed().TranslatePart().DistanceSq(currentCamera->cameraFrustum.pos);
		_orderedMeshes.emplace(std::make_pair(distance, &mesh.second));
	}

	// iterate meshes from furthest to closest.
	for (auto mesh = _orderedMeshes.rbegin(); mesh != _orderedMeshes.rend(); mesh++)
	{
		// Do camera culling checks first
		if (currentCamera->isCullingActive)
		{
			if (!currentCamera->IsInsideFrustum(mesh->second->globalAABB))
			{
				mesh->second->outOfFrustum = true;
				continue;
			}
			else
				mesh->second->outOfFrustum = false;
		}
		else if (currentCamera->type != CameraType::SCENE)
		{
			mesh->second->outOfFrustum = false;
		}

		// Update mesh. If the mesh should draw this frame, call Draw.
		if (mesh->second->Update())
		{
			mesh->second->Draw();
		}
	}

	_orderedMeshes.clear();
}

void RenderManager::DrawIndependentMeshes()
{
	if (_independentMeshes.empty())
		return;

	CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;
	for (auto& mesh : _independentMeshes)
	{
		// Do camera culling checks first
		if (currentCamera->isCullingActive)
		{
			if (!currentCamera->IsInsideFrustum(mesh.second.globalAABB))
			{
				mesh.second.outOfFrustum = true;
				continue;
			}
			else
				mesh.second.outOfFrustum = false;
		}
		else if (currentCamera->type != CameraType::SCENE)
		{
			mesh.second.outOfFrustum = false;
		}

		// Update mesh. If the mesh should draw this frame, call Draw.
		if (mesh.second.Update())
		{
			mesh.second.Draw();
		}
	}

}
