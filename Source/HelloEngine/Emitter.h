#pragma once
#include "Particle.h"
#include "Algorithm/Random/LCG.h"

class Shader;
class Application;
class InstanceRenderer;
class ParticleSystemComponent;
class ParticleManager;

class Emitter
{
public:

	Emitter();

	~Emitter();

	//void Draw(Shader* shader, Quat BBrot);

	void Draw();

	void InitBuffers();

	void EmitParticles(ParticleProperties& properties);

	void UpdateParticles();

	void SetParticlePoolSize(uint size);

public:

	int64_t _meshID = -1;

private:

	float3 position;

	std::vector<Particle> ParticleList;

	Application* app;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	LCG random;

	ParticleSystemComponent* component = nullptr;

	InstanceRenderer* manager;

	int currentparticle;

	friend class ParticleSystemComponent;
	friend class ParticleManager;

};

