#include "ModuleScene.h"
#include "Scene.h"
#include "Smoke.h"

Smoke::Smoke(Application* _app, float lifeTime, vec3 position, Color color) :GameObject("smoke", Tag::None, _app)
{
	this->maxLifeTime = this->lifeTime = lifeTime;

	shape.color = color;

	// random radiu
	float r = (rand() % 5) + 1;
	r /= 10;
	maxRadiu = r + 0.5f;

	shape.radius = r;

	this->position = position;

	shape.SetPos(position.x, position.y, position.z);

	Scene* scene = (Scene*)_app->scene->scenes[(int)_app->scene->currentScene];
	scene->AddGameObject(this);
}

Smoke::~Smoke()
{
}

void Smoke::Start()
{
}

void Smoke::Update()
{
	lifeTime -= _app->fps;

	position.y += _app->fps*5;

	if (shape.radius < 1) shape.radius += _app->fps/2;

	shape.SetPos(position.x, position.y, position.z);

	if (lifeTime <= 0) pendingToDelete = true;
}

void Smoke::PostUpdate()
{
	shape.color.a = abs(lifeTime / maxLifeTime);

	if(!pendingToDelete)
	{
		shape.Render();
	}
}

void Smoke::OnCollisionEnter(PhysBody3D* col)
{
}

void Smoke::OnTriggerEnter(PhysBody3D* col)
{
}
