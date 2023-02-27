#pragma once
#include "Module.h"

#include <AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>// Sample low-level I/O implementation

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	bool Init() override;

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	bool CleanUp() override;


private:

	bool InitSoundEngine();


	//// We're using the default Low-Level I/O implementation that's part
	//// of the SDK's sample code, with the file package extension
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
};

