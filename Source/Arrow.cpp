#include "Arrow.h"
#include "TaxiClient.h"
#include "Primitive.h"
#include "PhysBody3D.h"

Arrow::Arrow(std::string name, Tag tag, Application* app, GameObject* base, TaxiClient* target) : GameObject(name, tag, app)
{
	this->base = base;
	this->target = target;

	primitives[0] = new Cylinder(0.3f, 3);
	primitives[1] = new Sphere(0.5f);
	primitives[1]->color = { 1,0,0,1 };

}

void Arrow::Update()
{
	vec3 basePos = base->GetPosition();

	vec3 direction = *target->originPos - basePos;
	if (target->onTaxi)
	{
		direction = *target->destinationPos - basePos;
	}


	
	direction = normalize(direction);

	float rotation = atan2f(direction.x, direction.z);

	primitives[0]->SetPos(basePos.x, basePos.y+3, basePos.z);
	primitives[0]->SetRotation(rotation * RADTODEG + 90, { 0, 1, 0 });
	primitives[1]->SetPos(basePos.x + direction.x * 2, basePos.y+3, basePos.z + direction.z * 2);
}

void Arrow::PostUpdate()
{
	primitives[0]->Render();
	primitives[1]->Render();
}

void Arrow::CleanUp()
{
	RELEASE(primitives[0]);
	RELEASE(primitives[1]);
}

