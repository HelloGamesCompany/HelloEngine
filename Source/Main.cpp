#include "Headers.h"
#include <stdlib.h>
#include "Globals.h"
#include "Application.h"
#include "MemLeaks.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};


int main(int argc, char** argv)
{
	srand((unsigned)time(0));
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* app = nullptr;

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		LOG("Wrong DevIL version detected.");
		return 0;
	}

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			app = Application::Instance();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (app->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = (int)app->Update();

			if (update_return == (int)UpdateStatus::UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == (int)UpdateStatus::UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (app->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete app;
	app = nullptr;
	ilShutDown();
	ReportMemoryLeaks();

	LOG("\nBye :)\n");

	return main_return;
}