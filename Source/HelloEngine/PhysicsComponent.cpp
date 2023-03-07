#include "Headers.h"
#include "PhysicsComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Component::Type::PHYSICS;
	physBody = nullptr;
	_needsTransformCallback = true;

	shapeSelected = ColliderShape::NONE;

	isShapeSelected[0] = false;
	isShapeSelected[1] = false;
	isShapeSelected[2] = false;

	isShapeCreated[0] = false;
	isShapeCreated[1] = false;
	isShapeCreated[2] = false;

	sphereRadius = 1;
	cylRadiusHeight = {1,1};

	renderColColor[0] = 0.5f;
	renderColColor[1] = 0.0f;
	renderColColor[2] = 0.5f;
	renderColColor[3] = 1.0f;

	wireframeSize = 3.0f;

	sphereVerSlices = 16;
	sphereHorSlices = 16;
	cylinderVerSlices = 16;

	gravity[0] = { 0 };
	gravity[1] = { -9.8 };
	gravity[2] = { 0 };

	globalGravity[0] = { 0 };
	globalGravity[1] = { -9.8 };
	globalGravity[2] = { 0 };

	localGlobalGravity[0] = { 0 };
	localGlobalGravity[1] = { -9.8 };
	localGlobalGravity[2] = { 0 };
}

PhysicsComponent::~PhysicsComponent()
{
	if (physBody != nullptr)
		RemoveCollider();
}

void PhysicsComponent::SetVelocity(float3 vec)
{
	if (!physBody)
		return;

	physBody->SetVelocity(vec.x, vec.y, vec.z);
}

float3 PhysicsComponent::GetVelocity()
{
	return physBody->GetVelocity();
}

void PhysicsComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	
	_j["Enabled"] = _isEnabled;

	if (physBody != nullptr) {
		_j["HasPhysBody"] = true;
		_j["IsRenderingCol"] = physBody->isRenderingCol;
	}
	else {
		_j["HasPhysBody"] = false;
		_j["IsRenderingCol"] = false;
	}

	_j["ShapeSelected"] = shapeSelected;

	_j["IsShapeSelected"] = { isShapeSelected[0], isShapeSelected[1], isShapeSelected[2] };
	_j["IsShapeCreated"] = { isShapeCreated[0], isShapeCreated[1], isShapeCreated[2] };

	float3 getGrav = Application::Instance()->physic->GetGlobalGravity();
	_j["GlobalGravity"] = { getGrav[0], getGrav[1], getGrav[2] };

	if (physBody != nullptr)
	{
		_j["Mass"] = physBody->mass;

		_j["ColPosition"] = { physBody->colPos[0], physBody->colPos[1], physBody->colPos[2] };
		_j["ColRotation"] = { physBody->colRot[0], physBody->colRot[1],physBody->colRot[2] };
		_j["ColScale"] = { physBody->colScl[0], physBody->colScl[1], physBody->colScl[2] };

		_j["IsStatic"] = physBody->isStatic;
		_j["IsKinematic"] = physBody->isKinematic;
		_j["IsTrigger"] = physBody->isTrigger;

		_j["Gravity"] = { gravity[0], gravity[1], gravity[2] };
		
	}
	else
	{
		_j["Mass"] = 1.f;

		_j["ColPosition"] = {0, 0, 0 };
		_j["ColRotation"] = { 0, 0, 0 };
		_j["ColScale"] = { 0, 0, 0 };

		_j["IsStatic"] = false;
		_j["IsKinematic"] = false;
		_j["IsTrigger"] = false;
	}

	_j["SphereRadius"] = sphereRadius;

	_j["CylinderRadiusHeight"] = { cylRadiusHeight[0], cylRadiusHeight[1]};

	j["Components"].push_back(_j);
}

void PhysicsComponent::DeSerialization(json& j)
{
	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	shapeSelected = j["ShapeSelected"];

	std::vector<bool> isShapeSelectedTemp = j["IsShapeSelected"];
	isShapeSelected[0] = isShapeSelectedTemp[0];
	isShapeSelected[1] = isShapeSelectedTemp[1];
	isShapeSelected[2] = isShapeSelectedTemp[2];

	std::vector<bool> isShapeCreatedTemp = j["IsShapeCreated"];
	isShapeCreated[0] = isShapeCreatedTemp[0];
	isShapeCreated[1] = isShapeCreatedTemp[1];
	isShapeCreated[2] = isShapeCreatedTemp[2];

	sphereRadius = j["SphereRadius"];

	std::vector<float> cylRadiusHeightTemp = j["CylinderRadiusHeight"];
	cylRadiusHeight = { cylRadiusHeightTemp[0], cylRadiusHeightTemp[1] };

	std::vector<float> newGrav = j["GlobalGravity"];
	ModulePhysics::SetGlobalGravityAtFirst(float3(newGrav[0], newGrav[1], newGrav[2]));

	globalGravity[0] = newGrav[0];
	globalGravity[1] = newGrav[1];
	globalGravity[2] = newGrav[2];

	localGlobalGravity[0] = newGrav[0];
	localGlobalGravity[1] = newGrav[1];
	localGlobalGravity[2] = newGrav[2];

	if (j["HasPhysBody"] == true)
	{
		temporalMass = j["Mass"];
		CreateCollider();
		physBody->mass = temporalMass;

		physBody->isStatic = j["IsStatic"];
		physBody->isKinematic = j["IsKinematic"];
		physBody->isTrigger = j["IsTrigger"];

		std::vector<float> newGrav = j["Gravity"];
		gravity[0] = newGrav[0];
		gravity[1] = newGrav[1];
		gravity[2] = newGrav[2];
		physBody->SetGravity(float3(gravity[0], gravity[1], gravity[2]));
		
		physBody->isRenderingCol = j["IsRenderingCol"];
		std::vector<float> colPosTemp = j["ColPosition"];
		physBody->colPos = { colPosTemp[0], colPosTemp[1], colPosTemp[2] };

		std::vector<float> colRotTemp = j["ColRotation"];
		physBody->colRot = { colRotTemp[0], colRotTemp[1], colRotTemp[2] };

		std::vector<float> colSclTemp = j["ColScale"];
		physBody->colScl = { colSclTemp[0], colSclTemp[1], colSclTemp[2] };

		

		CallUpdateMass();
		CallUpdateColliderType();

		CallUpdatePos();
		CallUpdateRotation();
		CallUpdateScale();
	}
}
#ifdef STANDALONE
void PhysicsComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Physics", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Global Gravity", localGlobalGravity, 0.01)) {
			
			if ((localGlobalGravity[0] != globalGravity[0]) || (localGlobalGravity[1] != globalGravity[1]) || (localGlobalGravity[2] != globalGravity[2])) {
				//if (ImGui::Button("Apply")) {
					Application::Instance()->physic->PrepareNewGravityAtLast(float3(localGlobalGravity[0], localGlobalGravity[1], localGlobalGravity[2]));
				//}

			}
		}
		
		

		if (ImGui::DragFloat("Mass: ", &temporalMass, 0.01, 0, 99999.999)) {
			if (physBody != nullptr)
			{
				physBody->mass = temporalMass;
				CallUpdateMass();
			}
		};

		if (physBody == nullptr)
		{
			ImGui::Text("This object has no collider attached to it.");
			ImGui::Text("Select one shape and create one.");

			ImGui::Checkbox("Box", &isShapeSelected[0]);

			ImGui::Checkbox("Sphere", &isShapeSelected[1]);

			ImGui::Checkbox("Cylinder", &isShapeSelected[2]);

			CheckShapes();

			if (shapeSelected != ColliderShape::NONE) {
				if (ImGui::Button("Create Collider"))
				{
					CreateCollider();
					physBody->isRenderingCol = true;
				}
			}

		}
		else {
			const char* colliderType = "";
			switch (shapeSelected)
			{
			case ColliderShape::BOX:
			{
				colliderType = "Box";
			}
			break;
			case ColliderShape::SPHERE:
			{
				colliderType = "Sphere";
			}
			break;
			case ColliderShape::CYLINDER:
			{
				colliderType = "Cylinder";
			}
			break;
			default:
				break;
			}
			std::string colName = "Collider Attached: ";
			colName += colliderType;
			ImGui::Text(colName.c_str());

			if (ImGui::Checkbox("Static", &physBody->isStatic)) {
				if (physBody != nullptr) {
					if (physBody->isStatic == true) {
						physBody->isKinematic = false;
					}
					CallUpdateColliderType();
				}
			}

			if (ImGui::Checkbox("Kinematic", &physBody->isKinematic)) {
				if (physBody != nullptr) {
					if (physBody->isKinematic == true) {
						physBody->isStatic = false;
					}
					CallUpdateColliderType();
				}
			}

			if (ImGui::Checkbox("Trigger", &physBody->isTrigger)) {
				if (physBody != nullptr) {
					CallUpdateColliderType();
				}
			}

			if (ImGui::DragFloat3("Gravity", gravity, 0.01)) {
				if (physBody != nullptr) {
					physBody->SetGravity(float3(gravity[0], gravity[1], gravity[2]));
				}
			}

			if (ImGui::Checkbox("Render", &physBody->isRenderingCol)) {
				if (physBody->isRenderingCol == true) {
					switch (shapeSelected)
					{
					case ColliderShape::SPHERE:
					{
						if (sphereVerSlices < 3) {
							sphereVerSlices = 3;
						}
						if (sphereVerSlices > MAX_VERTICAL_SLICES_SPHERE) {
							sphereVerSlices = MAX_VERTICAL_SLICES_SPHERE;
						}

						if (sphereHorSlices < 1) {
							sphereHorSlices = 1;
						}
						if (sphereHorSlices > MAX_HORIZONTAL_SLICES_SPHERE) {
							sphereHorSlices = MAX_HORIZONTAL_SLICES_SPHERE;
						}
						Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(sphereVerSlices, sphereHorSlices);

					}
					break;
					case ColliderShape::CYLINDER:
					{
						if (cylinderVerSlices < 3) {
							cylinderVerSlices = 3;
						}
						if (cylinderVerSlices > MAX_VERTICAL_SLICES_CYLINDER) {
							cylinderVerSlices = MAX_VERTICAL_SLICES_CYLINDER;
						}
						Application::Instance()->renderer3D->renderManager.CalculateCylinderBuffer(cylinderVerSlices);
					}
					break;
					default:
						break;
					}
				}

			}

			if (physBody->isRenderingCol == true) {

				if (ImGui::ColorEdit4("Color", renderColColor)) {

				}
				if (ImGui::DragFloat("Line Size: ", &wireframeSize, 0.1f, 0.5f, 100.f)) {

				}

				switch (shapeSelected)
				{
				case ColliderShape::SPHERE:
				{
					if (ImGui::DragInt("Ver. Slices: ", &sphereVerSlices, 1, 3, MAX_VERTICAL_SLICES_SPHERE)) {
						Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(sphereVerSlices, sphereHorSlices);
					}
					if (ImGui::DragInt("Hor. Slices: ", &sphereHorSlices, 1, 1, MAX_HORIZONTAL_SLICES_SPHERE)) {
						Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(sphereVerSlices, sphereHorSlices);
					}
				}
				break;
				case ColliderShape::CYLINDER:
				{
					if (ImGui::DragInt("Ver. Slices: ", &cylinderVerSlices, 1, 3, MAX_VERTICAL_SLICES_CYLINDER)) {
						Application::Instance()->renderer3D->renderManager.CalculateCylinderBuffer(cylinderVerSlices);
					}
				}
				break;
				default:
					break;
				}
			}


			switch (shapeSelected)
			{
			case ColliderShape::BOX:
			{
				if (ImGui::DragFloat3("Position: ", physBody->colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", physBody->colRot.ptr(), 0.1)) {
					CallUpdateRotation();
				}

				if (ImGui::DragFloat3("Scale: ", physBody->colScl.ptr(), 0.1)) {
					CallUpdateScale();
				}
			}
			break;
			case ColliderShape::SPHERE:
			{
				if (ImGui::DragFloat3("Position: ", physBody->colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", physBody->colRot.ptr(), 0.1)) {
					//CallUpdatePos();
				}
				if (ImGui::DragFloat("Radius: ", &sphereRadius, 0.1)) {
					//CallUpdatePos();
				}
			}
			break;
			case ColliderShape::CYLINDER:
			{

				if (ImGui::DragFloat3("Position: ", physBody->colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}
				if (ImGui::DragFloat3("Rotation: ", physBody->colRot.ptr(), 0.1)) {
					//	CallUpdateShape();
				}
				if (ImGui::DragFloat2("Radius & Height: ", cylRadiusHeight.ptr(), 0.1)) {
					//CallUpdateShape();
				}
			}
			break;
			default:
				break;
			}

			if (ImGui::Button("Remove Collider"))
			{

				RemoveCollider();

			}
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}
#endif // STANDALONE



void PhysicsComponent::CallUpdatePos()
{
	ModulePhysics::UpdatePhysBodyPos(physBody);
}

void PhysicsComponent::CallUpdateRotation()
{
	ModulePhysics::UpdatePhysBodyRotation(physBody);
}

void PhysicsComponent::CallUpdateScale()
{
	ModulePhysics::UpdatePhysBodyScale(physBody);
}

void PhysicsComponent::CallUpdateMass()
{
	if (physBody->isStatic == false && physBody->isKinematic == false) {
		btVector3 inertia;
		btCollisionShape* shape = physBody->body->getCollisionShape();
		shape->calculateLocalInertia(physBody->mass, inertia);
		physBody->body->setMassProps(physBody->mass, inertia);
	}
	else {
		btVector3 inertia(0,0,0);
		physBody->body->setMassProps(0, inertia);
	}
}

void PhysicsComponent::CallUpdateStatic()
{
	if (physBody->isStatic == true) {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}
	else {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
	}
	CallUpdateMass();
}

void PhysicsComponent::CallUpdateTrigger()
{
	if (physBody->isTrigger == true) {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	else {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysicsComponent::CallUpdateKinematic()
{
	if (physBody->isKinematic == true) {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	else {
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	CallUpdateMass();
}

void PhysicsComponent::CallUpdateColliderType()
{
	CallUpdateStatic();
	CallUpdateKinematic();
	CallUpdateTrigger();

	CallUpdatePos();
	CallUpdateRotation();
	CallUpdateScale();
}

void PhysicsComponent::CreateCollider()
{

	switch (shapeSelected)
	{
		case ColliderShape::BOX:
		{
			PrimCube cube;
			cube.transform = cube.transform.identity;

			cube.size.x = 1;//GetScale().x;
			cube.size.y = 1;
			cube.size.z = 1;
			//cube.transform.Translate(_gameObject->GetComponent<TransformComponent>()->GetGlobalPosition());

			physBody = Application::Instance()->physic->CreatePhysBody(&cube, 1);

		}
		break;
		case ColliderShape::SPHERE:
		{
			PrimSphere sphere;

			sphere.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y * WORLD_TO_RENDER_RELATION;

			physBody = Application::Instance()->physic->CreatePhysBody(&sphere, 1);

		}
		break;
		case ColliderShape::CYLINDER:
		{
			PrimCylinder cylinder;

			cylinder.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().x * WORLD_TO_RENDER_RELATION;
			cylinder.height = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y * WORLD_TO_RENDER_RELATION;

			physBody = Application::Instance()->physic->CreatePhysBody(&cylinder, 1);

		}
		break;
		default:
			break;
	}

	physBody->gameObjectUID = _gameObject->GetID();
	physBody->SetPos(physBody->colPos.x, physBody->colPos.y, physBody->colPos.z);
	CallUpdatePos();
}

void PhysicsComponent::RemoveCollider()
{
	ModulePhysics::S_RemovePhysBody(physBody);
	physBody = nullptr;
}

void PhysicsComponent::OnTransformCallback(float4x4 worldMatrix)
{
	if (_gameObject->transform->_ignorePhysBody) // If this transformation has to be ignored...
	{
		_gameObject->transform->_ignorePhysBody = false; // ... set the flag to false, so next transformation doesn't get ignored, and return.
		return;
	}

	if (physBody != nullptr)
	{
		CallUpdatePos();
		CallUpdateRotation();
	}
}

void PhysicsComponent::CheckShapes() {

	for (int h = 0; h < static_cast<int>(ColliderShape::Count); h++) {


		if (isShapeSelected[h] == true) {
			if (isShapeCreated[h] == false)
			{
				if (shapeSelected != ColliderShape::NONE && shapeSelected != static_cast<ColliderShape>(h)) {

				}
				shapeSelected = static_cast<ColliderShape>(h);
				isShapeCreated[h] = true;
			}
		}
		else {
			if (isShapeCreated[h] == true) {

			}
		}

	}

	int a = 0;
	for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
		if (isShapeSelected[i] == true)
		{
			a++;
		}
	}

	if (a == 0) {
		shapeSelected = ColliderShape::NONE;
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
			if (isShapeCreated[i] == true)
			{
				isShapeCreated[i] = false;
			}
		}
	}

	if (shapeSelected == ColliderShape::NONE)
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {
			if (isShapeSelected[i] == true) {
				isShapeSelected[i] = false;
				if (isShapeCreated[i] == true) {
					isShapeCreated[i] = false;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {

			if (i != static_cast<int>(shapeSelected)) {
				if (isShapeSelected[i] == true) {
					isShapeSelected[i] = false;
					if (isShapeCreated[i] == true) {
						isShapeCreated[i] = false;
					}
				}
			}

		}
	}
}
