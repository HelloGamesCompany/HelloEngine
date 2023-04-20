#pragma once

#include "LightComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "SpotLightComponent.h"

struct GlobalLight
{
	float4 lightDirection = float4(0.2f, 1.0f, -0.75f, 0.0f);
	float3 lightColor = float3(1.0, 1.0f, 1.0f);

	float lightStrength = 1.0f;
};

struct LightMap
{
	DirectionalLight directionalLight;
	std::map<uint, PointLight> pointLight;
	std::map<uint, SpotLight> spotLight;
};

class Lighting
{
public:
	Lighting();
	~Lighting();

	void Update();

	//uint AddLight(Light lightData);
	static uint AddSpotLight(SpotLight lightData);
	static uint AddPointLight(PointLight lightData);
	static void SetDirectionalLight(DirectionalLight lightData);
	
	static LightMap& GetLightMap() { return _lightMap; }

private:
	bool _active;

	int ref;

	static LightMap _lightMap;

public:
	static GlobalLight global;
};

