#include "Headers.h"
#include "UUID.h"


LCG HelloUUID::lcg;
std::vector<uint> HelloUUID::generatedUIDs;
SHA256 HelloUUID::hash;

uint HelloUUID::GenerateUUID()
{
    uint id = lcg.IntFast();
    auto res = std::find(generatedUIDs.begin(), generatedUIDs.end(), id);
    if (res != generatedUIDs.end())
    {
        return GenerateUUID();
    }
    generatedUIDs.push_back(id);
    return id;
}

std::string HelloUUID::GenerateGUID(std::string input)
{
    return hash(input);
}

