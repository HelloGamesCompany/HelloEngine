#pragma once

struct GlobalLight
{
	float4 lightDirection = float4(0.2f, 1.0f, -0.75f, 0.0f);
	float3 lightColor = float3(1.0, 1.0f, 1.0f);

	float lightStrength = 1.0f;
};

class Lighting
{
public:
	Lighting();
	~Lighting();

	void Update();

private:
	bool _active;

	int ref;

public:
	static GlobalLight global;
};

