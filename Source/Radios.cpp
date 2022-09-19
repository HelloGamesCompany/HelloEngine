#include "Radios.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"

Radios::Radios(Application* _app) :GameObject("radios",Tag::None,_app)
{

}

void Radios::Start()
{
	// init all audios
	playLists[0].path = "Assets/Audios/PlayList1.ogg";
	playLists[0].duration = 745;

	playLists[1].path = "Assets/Audios/PlayList2.ogg";
	playLists[1].duration = 621;

	playLists[2].path = "Assets/Audios/PlayList3.ogg";
	playLists[2].duration = 1043;

	playLists[3].path = "Assets/Audios/PlayList4.ogg";
	playLists[3].duration = 927;

	currentList = rand() % 4;

	_app->audio->PlayMusic(playLists[currentList].path.c_str());

	Mix_VolumeMusic(50);

	changeSceneSfxId = _app->audio->LoadFx("Assets/Audios/changeList.mp3",25) - 1;
}

void Radios::Update()
{
	timer += _app->fps;

	if (_app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		currentList--;

		currentList = currentList < 0 ? 3 : currentList;

		_app->audio->PlayFx(changeSceneSfxId, 0);

		ChangeList();
	}
	else if (_app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		currentList++;

		currentList = currentList > 3 ? 0 : currentList;

		_app->audio->PlayFx(changeSceneSfxId, 0);

		ChangeList();
	}

	if(_app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		enable = !enable;

		if(enable)
		{
			Mix_VolumeMusic(50);

			_app->audio->PlayFx(changeSceneSfxId, 0);
		}
		else
		{
			Mix_VolumeMusic(0);

			_app->audio->PlayFx(changeSceneSfxId, 0);
		}
	}
}

void Radios::OnCollisionEnter(PhysBody3D* col)
{
}

void Radios::OnTriggerEnter(PhysBody3D* col)
{
}

void Radios::ChangeList()
{
	timer = timer > playLists[currentList].duration ? (int)timer % playLists[currentList].duration : timer;

	_app->audio->PlayMusic(playLists[currentList].path.c_str(),0);

	Mix_SetMusicPosition(timer);
}
