#include "PlayAmbienceMusic.h"
HELLO_ENGINE_API_C PlayAmbienceMusic* CreatePlayAmbienceMusic(ScriptToInspectorInterface* script)
{
    PlayAmbienceMusic* classInstance = new PlayAmbienceMusic();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddInputBox("Ambience Sound", &classInstance->sound);
    script->AddDragFloat("Sound Duration", &classInstance->maxSoundDuration);
    return classInstance;
}

void PlayAmbienceMusic::Start()
{
    if (sound != "")
    {
        Audio::Event(sound.c_str());
        soundDuration = maxSoundDuration;
    }
}

void PlayAmbienceMusic::Update()
{
    //4.95
    if (sound != "")
    {
        soundDuration -= Time::GetRealTimeDeltaTime();
        if (soundDuration <= 0.0f)
        {
            Audio::Event(sound.c_str());
            soundDuration = maxSoundDuration;
        }
    }
}