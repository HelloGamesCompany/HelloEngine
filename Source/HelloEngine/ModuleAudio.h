#pragma once
#include "Module.h"

#include "Ak/samples/AkFilePackageLowLevelIOBlocking.h"// Sample low-level I/O implementation

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

	AkGameObjectID RegisterGameObject(AkGameObjectID id);
	void SetDefaultListener(const AkGameObjectID id);


private:

	bool InitSoundEngine();
	void TerminateSoundEngine();

	void ProcessAudio();

	void InitSoundBanks();
	void TerminateSoundBanks();

	//// We're using the default Low-Level I/O implementation that's part
	//// of the SDK's sample code, with the file package extension
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	AkBankID _initBank;
	AkBankID _testBank;

	// Temporal
	bool isPlayingBackground = false;

};

