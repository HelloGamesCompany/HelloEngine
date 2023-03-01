#pragma once
#include "Particle.h"
#include "Algorithm/Random/LCG.h"

class Shader;
class Application;

class Emitter
{
public:

	Emitter();

	~Emitter();

	void Draw(Shader* shader, Quat BBrot);

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

	

	int currentparticle;

};

