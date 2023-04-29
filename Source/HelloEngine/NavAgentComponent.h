#pragma once
#include "Component.h"

struct NavAgent;
class Pathfinder;

class ComponentAgent : public Component
{
public: 
	ComponentAgent(GameObject* gameObject);

	~ComponentAgent();

#ifdef STANDALONE
	void OnEditor() override;
#endif // STANDALONE

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void SetTarget(float3 targetPos);

public:
	NavAgent* agentProperties = nullptr;
	Pathfinder* pathfinder = nullptr;

private:
	float3 _target;
};

