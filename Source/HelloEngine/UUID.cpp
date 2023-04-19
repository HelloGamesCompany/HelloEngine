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

uint HelloUUID::GenerateGUID(std::string input)
{
    std::string key = hash(input);
    key = hash(key);
    uint hashValue = 0;
    for (int i = 0; i < key.size(); ++i) {
        hashValue = (hashValue << 8) | key[i];
    }

    return hashValue;
}

