#include "Headers.h"	
#include "API_Audio.h"
#include "ModuleAudio.h"

void API::Audio::Event(const char* eventName)
{
	ModuleAudio::S_ProduceEvent(eventName);
}

void API::Audio::Event(uint eventID)
{
	ModuleAudio::S_ProduceEvent(eventID);
}
