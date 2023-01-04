#include "BulletBehavior.h"
#include "API/API.h"
void BulletBehavior::Start()
{

}
void BulletBehavior::Update()
{
	lifeTime -= Time::GetDeltaTime();

	if (lifeTime <= 0)
	{
		this->gameObject.Destroy();
	}

	this->gameObject.GetTransform().Translate(0, 0, 0.2f);
}