#pragma once
#include "Globals.h"
#include "Algorithm/Random/LCG.h"
#include "sha256.h"

class HelloUUID
{
public:
	/// A UUID is not based in any input.
	static uint GenerateUUID();

	/// A GUID is based in an input, and will always return the same using the same input.
	static std::string GenerateGUID(std::string input);

private:
	static LCG lcg;
	static SHA256 hash;
	static std::vector<uint> generatedUIDs;
};

