#ifndef __RADIOS_H__
#define __RADIOS_H__

#include "GameObject.h"

struct PlayList
{
	std::string path = "node";
	int duration = 0;
};


class Radios : public GameObject
{
public:
	Radios(Application* _app);

	void Start() override;

	void Update() override;

	void OnCollisionEnter(PhysBody3D* col);

	void OnTriggerEnter(PhysBody3D* col);

private:
	void ChangeList();

private:

	PlayList playLists[4];

	float timer = 0.0f;

	int currentList = 0;

	int changeSceneSfxId = 0;

	bool enable = true;
};

#endif // !__RADIOS_H__