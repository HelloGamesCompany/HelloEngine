#ifndef __TAXICLIENT_H__
#define __TAXICLIENT_H__

#include "GameObject.h"
#include "glmath.h"

enum TaxiClientState
{
	WaitingTaxi = 0,
	OnTaxi
};


class TaxiClient : public GameObject
{
public:

	TaxiClient(std::string name, Tag tag, Application* _app);
	~TaxiClient();

	void Start() override;
	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	void OnTriggerEnter(PhysBody3D* col) override;

	void generatePosition();

private:
	float height = 64.0f;
	float radius = 16.0f;

	vec3 spawnPositions[12] = { vec3(240, 1, 206), vec3(540, 1, 195), vec3(900, 1, 200), vec3(836, 1, 66), vec3(270, 1, 21), vec3(23, 1, 150),
								 vec3(158, 1, 649), vec3(247, 1, 1033), vec3(560, 1, 960), vec3(550, 1, 690), vec3(1000, 1, 700), vec3(1312, 1, 910) };
	
public:

	int serviceCount = 0;
	bool onTaxi = false;

	vec3* originPos = nullptr;
	vec3* destinationPos = nullptr;

	PhysBody3D* pBody2 = nullptr;

public:
	/*GETTERS AND SETTERS*/

	void SetHeight(float height)
	{
		this->height = height;
	}

	float GetHeight()
	{
		return height;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

	float GetRadius()
	{
		return radius;
	}

};

#endif //!__TAXICLIENT_H__
