#ifndef __WEELPRINT_H__
#define __WEELPRINT_H_

#include "Primitive.h"
#include "GameObject.h"

class WeelPrint :  public GameObject
{
public:
	WeelPrint(Application* _app, float lifeTime, mat4x4 transform, Color color = Black);
	~WeelPrint();

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

	Cube shape;
};

#endif // !__WEELPRINT_H__
