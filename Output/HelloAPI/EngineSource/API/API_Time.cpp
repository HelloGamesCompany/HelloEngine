#include "Headers.h"
#include "API_Time.h"
#include "EngineTime.h"

float API::Time::GetDeltaTime()
{
	return EngineTime::GameDeltaTime();
}

float API::Time::GetRealTimeDeltaTime()
{
	return EngineTime::RealTimeDeltaTime();
}

TO_API void API::Time::ChangeTimeScale(float timeScale)
{
	EngineTime::SetTimeScale(timeScale);
}

TO_API float API::Time::GetTimeScale()
{
	return EngineTime::GetTimeScale();
}
