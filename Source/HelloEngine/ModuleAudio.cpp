#include "Headers.h"
#include "ModuleAudio.h"


#include <AK/SoundEngine/Common/AkMemoryMgr.h> // Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h> // Default memory manager
#include <AK/SoundEngine/Common/IAkStreamMgr.h> // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h> // Thread defines
//#include <AK/SoundEngine/Common/AkSoundEngine.h> // Sound Engine
//#include <AK/MusicEngine/Common/AkMusicEngine.h> // Music Engine
//#include <AK/SpatialAudio/Common/AkSpatialAudio.h> // Spatial Audio
//// Include for communication between Wwise and the game -- Not needed in the release version
//#ifndef AK_OPTIMIZED
//#include <AK/Comm/AkCommunication.h>
//#endif // AK_OPTIMIZED


ModuleAudio::ModuleAudio(bool start_enabled)
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
    InitSoundEngine();
    return true;
}

bool ModuleAudio::Start()
{
    return true;
}

UpdateStatus ModuleAudio::PreUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleAudio::Update()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleAudio::PostUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
    return true;
}

bool ModuleAudio::InitSoundEngine()
{
    AkMemSettings memSettings;
    AK::MemoryMgr::GetDefaultSettings(memSettings);

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        Console::S_Log("Could not create the memory manager.");
        return false;
    }

    //AkStreamMgrSettings stmSettings;
    //AK::StreamMgr::GetDefaultSettings(stmSettings);

    //// Customize the Stream Manager settings here.
    //if (!AK::StreamMgr::Create(stmSettings))
    //{
    //    Console::S_Log("Could not create the Streaming Manager");
    //    return false;
    //}
    //
    //AkDeviceSettings deviceSettings;
    //AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    //// Customize the streaming device settings here.
    //// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    //// in the Stream Manager, and registers itself as the File Location Resolver.

    //if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    //{
    //    Console::S_Log("Could not create the streaming device and Low-Level I/O system");
    //    return false;
    //}

    //// Create the Sound Engine
    //// Using default initialization parameters

    //AkInitSettings initSettings;
    //AkPlatformInitSettings platformInitSettings;
    //AK::SoundEngine::GetDefaultInitSettings(initSettings);
    //AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    //if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    //{
    //    Console::S_Log("Could not initialize the Sound Engine.");
    //    return false;
    //}

    //// Initialize the music engine
    //// Using default initialization parameters

    //AkMusicSettings musicInit;
    //AK::MusicEngine::GetDefaultInitSettings(musicInit);

    //if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    //{
    //    Console::S_Log("Could not initialize the Music Engine.");
    //    return false;
    //}

    // Initialize Spatial Audio
    // Using default initialization parameters

    //AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

    //if (AK::SpatialAudio::Init(settings) != AK_Success)
    //{
    //    Console::S_Log("Could not initialize the Spatial Audio.");
    //    return false;
    //}

//#ifndef AK_OPTIMIZED
//
//    // Initialize communications (not in release build!)
//    AkCommSettings commSettings;
//    AK::Comm::GetDefaultInitSettings(commSettings);
//    if (AK::Comm::Init(commSettings) != AK_Success)
//    {
//        Console::S_Log("Could not initialize communication.");
//        return false;
//    }
//#endif // AK_OPTIMIZEDs

    return true;
}
