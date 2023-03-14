#pragma once

#include <string>
#include <vector>
#include <cassert>
#include "MathGeoLib.h"
#include "ModuleXML.h"

class Uniform;

struct ShaderData
{
public:
	uint ID;
	std::string name;
	
	std::string vertexCode;
	std::string fragmentCode;

	std::vector<Uniform*> uniforms;
};