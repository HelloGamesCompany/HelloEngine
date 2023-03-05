#include "Headers.h"
#include "PhysicsComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Component::Type::PHYSICS;
	physBody = nullptr;

	mass = 0;

	shapeSelected = ColliderShape::NONE;

	isShapeSelected[0] = false;
	isShapeSelected[1] = false;
	isShapeSelected[2] = false;

	isShapeCreated[0] = false;
	isShapeCreated[1] = false;
	isShapeCreated[2] = false;

	isStatic = false;

	colPos = gameObject->GetComponent<TransformComponent>()->GetGlobalPosition();
	colRot = gameObject->GetComponent<TransformComponent>()->GetGlobalRotation();
	colScl = gameObject->GetComponent<TransformComponent>()->GetGlobalScale();

	sphereRadius = 0;
	cylRadiusHeight = {0,0};
}

PhysicsComponent::~PhysicsComponent()
{
	if (physBody != nullptr)
		RemoveCollider();
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

	 

	

	_j["Mass"] = mass;

	_j["ShapeSelected"] = shapeSelected;

	_j["IsShapeSelected"] = { isShapeSelected[0], isShapeSelected[1], isShapeSelected[2] };
	_j["IsShapeCreated"] = { isShapeCreated[0], isShapeCreated[1], isShapeCreated[2] };

	_j["IsStatic"] = isStatic;

	_j["ColPosition"] = { colPos[0], colPos[1], colPos[2] };
	_j["ColRotation"] = { colRot[0], colRot[1], colRot[2] };
	_j["ColScale"] = { colScl[0], colScl[1], colScl[2] };

	_j["SphereRadius"] = sphereRadius;

	_j["CylinderRadiusHeight"] = { cylRadiusHeight[0], cylRadiusHeight[1]};

	j["Components"].push_back(_j);
}

void PhysicsComponent::DeSerialization(json& j)
{
	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();


	


	mass = j["Mass"];

	shapeSelected = j["ShapeSelected"];

	std::vector<bool> isShapeSelectedTemp = j["IsShapeSelected"];
	isShapeSelected[0] = isShapeSelectedTemp[0];
	isShapeSelected[1] = isShapeSelectedTemp[1];
	isShapeSelected[2] = isShapeSelectedTemp[2];

	std::vector<bool> isShapeCreatedTemp = j["IsShapeCreated"];
	isShapeCreated[0] = isShapeCreatedTemp[0];
	isShapeCreated[1] = isShapeCreatedTemp[1];
	isShapeCreated[2] = isShapeCreatedTemp[2];

	isStatic = j["IsStatic"];

	

	std::vector<float> colPosTemp = j["ColPosition"];
	colPos = { colPosTemp[0], colPosTemp[1], colPosTemp[2] };

	std::vector<float> colRotTemp = j["ColRotation"];
	colRot = { colRotTemp[0], colRotTemp[1], colRotTemp[2] };

	std::vector<float> colSclTemp = j["ColScale"];
	colScl = { colSclTemp[0], colSclTemp[1], colSclTemp[2] };

	sphereRadius = j["SphereRadius"];

	std::vector<float> cylRadiusHeightTemp = j["CylinderRadiusHeight"];
	cylRadiusHeight = { cylRadiusHeightTemp[0], cylRadiusHeightTemp[1] };

	if (j["HasPhysBody"] == true)
	{
		CreateCollider();
		physBody->isRenderingCol = j["IsRenderingCol"];
	}

	

}

void PhysicsComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Static", &isStatic)) {
			if (physBody != nullptr) {
				CallUpdateMass();
			}
		}

		if (ImGui::DragFloat("Mass: ", &mass, 0.01)) {
			CallUpdateMass();
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

			if (ImGui::Checkbox("Render", &physBody->isRenderingCol)) {

			}

			switch (shapeSelected)
			{
			case ColliderShape::BOX:
			{
				if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
					//CallUpdatePos();
				}

				if (ImGui::DragFloat3("Scale: ", colScl.ptr(), 0.1)) {
					//CallUpdatePos();
				}
			}
			break;
			case ColliderShape::SPHERE:
			{
				if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
					//CallUpdatePos();
				}
				if (ImGui::DragFloat("Radius: ", &sphereRadius, 0.1)) {
					//CallUpdatePos();
				}
			}
			break;
			case ColliderShape::CYLINDER:
			{

				if (ImGui::DragFloat3("Position: ", colPos.ptr(), 0.1)) {
					CallUpdatePos();
				}
				if (ImGui::DragFloat3("Rotation: ", colRot.ptr(), 0.1)) {
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
}

void PhysicsComponent::CallUpdatePos()
{
	ModulePhysics::UpdatePhysBodyPos(physBody, colPos);
}

void PhysicsComponent::CallUpdateMass()
{
	if (isStatic == false) {
		btVector3 inertia(0, 0, 0);
		physBody->body->setMassProps(mass, inertia);
	}
	else {
		btVector3 inertia(0, 0, 0);
		physBody->body->setMassProps(0, inertia);
	}
}

void PhysicsComponent::CreateCollider()
{
	
	/*float mass;
	if (isStatic == true) {
		mass = 0.f;
	}
	else {
		mass = 1.f;
	}*/

	switch (shapeSelected)
	{
		case ColliderShape::BOX:
		{
			PrimCube cube;
			cube.transform = cube.transform.identity;

			cube.size.x = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().x;//GetScale().x;
			cube.size.y = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y;
			cube.size.z = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().z;
			//cube.transform.Translate(_gameObject->GetComponent<TransformComponent>()->GetGlobalPosition());

			physBody = Application::Instance()->physic->CreatePhysBody(&cube, mass);



		}
		break;
		case ColliderShape::SPHERE:
		{
			PrimSphere sphere;

			sphere.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y;

			physBody = Application::Instance()->physic->CreatePhysBody(&sphere, mass);

		}
		break;
		case ColliderShape::CYLINDER:
		{
			PrimCylinder cylinder;

			cylinder.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().x;
			cylinder.height = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y;

			physBody = Application::Instance()->physic->CreatePhysBody(&cylinder, mass);

		}
		break;
		default:
			break;
	}

	physBody->gameObjectUID = _gameObject->GetID();
	physBody->SetPos(colPos.x, colPos.y, colPos.z);
	

}

void PhysicsComponent::RemoveCollider()
{
	ModulePhysics::S_RemovePhysBody(physBody);
	physBody = nullptr;
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
