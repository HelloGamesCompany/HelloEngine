#include "Headers.h"
#include "Lighting.h"

GlobalLight Lighting::global;

Lighting::Lighting()
{
	_active = true;
	ref = -1;
}

Lighting::~Lighting()
{

}

void Lighting::Update()
{
	if (!_active) return;

	//float pos[] = { global.position.x, global.position.y, global.position.z, 1.0f };
	//glLightfv(ref, GL_POSITION, pos);
}