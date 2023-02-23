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
