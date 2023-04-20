#include "Headers.h"
#include "UUID.h"


LCG HelloUUID::lcg;
std::vector<uint> HelloUUID::generatedUIDs;
SHA256 HelloUUID::hash;

uint HelloUUID::GenerateUUID()
{
    uint id = lcg.IntFast();
    //auto res = std::find(generatedUIDs.begin(), generatedUIDs.end(), id);
    //if (res != generatedUIDs.end())
    //{
    //    return GenerateUUID();
    //}
    //generatedUIDs.push_back(id);
    return id;
}

uint HelloUUID::GenerateGUID(std::string input)
{
    std::string key = hash(input);
    key = hash(key);
    uint32_t hashValue = 0x811c9dc5;
    uint32_t prime = 0x1000193;

    for (int i = 0; i < key.size(); ++i) {
        uint8_t value = key[i];
        hashValue = hashValue ^ value;
        hashValue *= prime;
    }

    return hashValue;
}

