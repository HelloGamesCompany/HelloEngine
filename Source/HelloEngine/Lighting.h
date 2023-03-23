#pragma once

struct GlobalLight
{
	float3 lightPosition = float3(0.0f, 1.0f, -1.0f);
	float3 lightColor = float3(0.55f, 1.0f, 0.898f);

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

