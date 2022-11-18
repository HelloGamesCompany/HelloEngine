#pragma once
#include "Globals.h"
#include "Algorithm/Random/LCG.h"

class HelloUUID
{
public:
	static uint GenerateUUID();

private:
	static LCG lcg;
	static std::vector<uint> generatedUIDs;
};

