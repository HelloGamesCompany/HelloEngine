#pragma once

class ParticleSystemComponent;

class P_MainModule
{
public:

	P_MainModule();
	~P_MainModule();

	void OnEditor();

private:

	ParticleSystemComponent* component = nullptr;

	friend class ParticleSystemComponent;
};

