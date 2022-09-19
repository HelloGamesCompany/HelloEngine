#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include "Point.h"
#include "Application.h"
#include "glmath.h"

#define MAX_GAMEOBJECT_TEXTURES 10

class PhysBody3D;
class Primitive;

enum class Tag
{
	None = 0,
	Vehicle,
	TaxiClient,
	Wall,
	Booster
};

class GameObject 
{
public:

	GameObject();

	GameObject(std::string name = "Default", Tag tag = Tag::None, Application* _app = nullptr);

	GameObject(GameObject& obj);

	virtual ~GameObject();

	virtual void OnCollisionEnter(PhysBody3D* col);
	//virtual void OnCollisionExit(PhysBody* col);

	virtual void OnTriggerEnter(PhysBody3D* col);
	//virtual void OnTriggerStay(PhysBody* trigger, PhysBody* col);
	//virtual void OnTriggerExit(PhysBody* trigger, PhysBody* col);

	virtual void InitAsCube(float x, float y, float z, float mass);

	virtual void Start();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

	bool CompareTag(Tag tag);

	vec3 GetPosition() const;

private :
	

protected:
	vec3 position = { 0,0,0 };

	float rotation = 0;

	Application* _app = nullptr;

public:
	std::string name = "";

	Tag tag = Tag::None;

	PhysBody3D* pBody = nullptr;

	Primitive* primitive = nullptr;

	bool pendingToDelete = false;

	bool adjustToGrid = false;

	bool isDie = false;

	bool enable = true;
};

#endif // !__GAMEOBJECT_H__