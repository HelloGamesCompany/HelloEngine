#pragma once

#include <string>

struct ShaderData
{
public:
	uint ID;
	std::string name;
	
	std::string vertexCode;
	std::string fragmentCode;
};