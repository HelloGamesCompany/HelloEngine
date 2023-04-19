#pragma once

class ParticleSystemComponent;

enum class P_ModuleType
{
	MAIN,
	NONE
};

class P_Module
{
public:

	P_Module();
	virtual ~P_Module();

	virtual void OnEditor();

	ParticleSystemComponent* component = nullptr;

	P_ModuleType type;

	friend class ParticleSystemComponent;
};
