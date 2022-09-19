#include "GameObject.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, Tag tag , Application* _app)
{
	this->name = name;
	this->_app = _app;
	this->tag = tag;
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
	//this->pBody = obj.pBody;
}

GameObject::~GameObject()
{
}

//GameObject::~GameObject()
//{
//
//}

void GameObject::OnCollisionEnter(PhysBody3D* col)
{
}

//void GameObject::OnCollisionEnter(PhysBody* col)
//{
//}
//
//void GameObject::OnCollisionExit(PhysBody* col)
//{
//
//}
//
void GameObject::OnTriggerEnter(PhysBody3D* col)
{
}
//
//void GameObject::OnTriggerStay(PhysBody* trigger, PhysBody* col)
//{
//}
//
//void GameObject::OnTriggerExit(PhysBody* trigger, PhysBody* col)
//{
//}

void GameObject::InitAsCube(float x, float y, float z, float mass)
{
	this->primitive = new Cube(x, y, z);
	this->pBody = _app->physics->CreateCube(x, y, z, mass);
	this->pBody->gameObject = this;
}
void GameObject::Start()
{
	//Cargar texturas
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
	if (primitive != nullptr)
	{
		pBody->body->getCenterOfMassTransform().getOpenGLMatrix(&primitive->transform);
		primitive->Render();
	}
}

void GameObject::CleanUp()
{
	RELEASE(primitive);
}

bool GameObject::CompareTag(Tag tag)
{
	if(this->tag == tag)
	{
		return true;
	}

	return false;
}

vec3 GameObject::GetPosition() const
{
	return pBody->GetPos();
}
