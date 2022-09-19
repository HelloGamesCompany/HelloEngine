#include "ModuleScene.h"
#include "Scene.h"
#include "WeelPrint.h"

WeelPrint::WeelPrint(Application* _app, float lifeTime, mat4x4 transform, Color color) :GameObject("smoke", Tag::None, _app)
{
	this->maxLifeTime = this->lifeTime = lifeTime;

	shape.color = color;

	shape.size = {0.5, 0.1, 2};

	shape.transform = transform;

	Scene* scene = (Scene*)_app->scene->scenes[(int)_app->scene->currentScene];
	scene->AddGameObject(this);
}

WeelPrint::~WeelPrint()
{
}

void WeelPrint::Start()
{
}

void WeelPrint::Update()
{
	lifeTime -= _app->fps;

	position.y += _app->fps * 5;

	if (lifeTime <= 0) pendingToDelete = true;
}

void WeelPrint::PostUpdate()
{
	if (!pendingToDelete)
	{
		shape.Render();
	}
}

void WeelPrint::OnCollisionEnter(PhysBody3D* col)
{
}

void WeelPrint::OnTriggerEnter(PhysBody3D* col)
{
}
