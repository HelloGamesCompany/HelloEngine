#include "Booster.h"
#include "ModulePhysics3D.h"

Booster::Booster(std::string name, Tag tag, Application* app, vec3 pos, vec3 dimensions, bool boost) : GameObject(name, tag, app)
{
	this->pBody = _app->physics->CreateCubeArea(dimensions.x, dimensions.y, dimensions.z, pos);
	this->pBody->gameObject = this;
	this->boost = boost;

	this->primitive = new Cube(dimensions.x, dimensions.y, dimensions.z);
	this->primitive->color = boost ? Color(1, 0.5, 0, 0.6) : Color(0,0,1, 0.6);

}
