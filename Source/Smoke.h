#ifndef __SMOKE_H__
#define __SMOKE_H_

#include "Primitive.h"
#include "GameObject.h"

class Smoke : public GameObject
{
public:
	Smoke(Application* _app, float lifeTime, vec3 position, Color color = Black);
	~Smoke();

	void Start() override;
	void Update() override;
	void PostUpdate() override;

	void OnCollisionEnter(PhysBody3D* col) override;
	void OnTriggerEnter(PhysBody3D* col) override;

private:
	// second
	float lifeTime = 1;
	float maxLifeTime = 1;
	float maxRadiu = 1;
	
	vec3 position;

	Sphere shape;
};

#endif // !__SMOKE_H__