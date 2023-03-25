#pragma once
#include "Globals.h"

namespace API
{
	namespace Time
	{
		TO_API float GetDeltaTime();
		TO_API float GetRealTimeDeltaTime();
		TO_API void ChangeTimeScale(float timeScale);
		TO_API float GetTimeScale();
	}
}