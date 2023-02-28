#pragma once
#include <vector>

class Emitter;
class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void Init();
	void Update();
	void Draw();

	std::vector<Emitter*> EmitterList;
};

