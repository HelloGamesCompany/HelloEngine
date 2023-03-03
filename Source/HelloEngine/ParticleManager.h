#pragma once
#include <vector>
#include "Particle.h"

class Emitter;
class ParticleManager
{
public:

	ParticleManager();
	~ParticleManager();

	void Init();
	void Update(Emitter& emitter);
	void Draw();

	void RemoveEmitterInList(Emitter* emitter);

	std::vector<Emitter*> EmitterList;

};

