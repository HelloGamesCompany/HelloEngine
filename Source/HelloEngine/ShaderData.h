#pragma once

#include <string>

struct ShaderData
{
public:
	uint ID = 0;
	std::string name;
	
	std::string vertexCode;
	std::string fragmentCode;

	bool isIstanced = false;
};