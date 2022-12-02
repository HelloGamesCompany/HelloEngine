#include "Headers.h"


// Real Time
float Time::_realTimeDeltaTime = 0;
uint Time::_lastFrameTicks = 0;
double Time::_realTimeTotalTime = 0;

// Game Time
float Time::_gameTimeDeltaTime = 0;
uint Time::_frameCount = 0;
double Time::_gameTimeTotalTime = 0;
float Time::_timeScale = 1.0f;

void Time::UpdateRealTime()
{
	// Get real time in seconds (SDL_GetTicks() returns in miliseconds)
	_realTimeDeltaTime = (SDL_GetTicks() - _lastFrameTicks) * 0.001f;
	_lastFrameTicks = SDL_GetTicks();
	_realTimeTotalTime += _realTimeDeltaTime;

}

void Time::UpdateGameTime()
{
	_gameTimeDeltaTime = RealTimeDeltaTime() * _timeScale;
	_gameTimeTotalTime += _gameTimeDeltaTime;
	// Add frame count
	_frameCount++;
}

float Time::GameDeltaTime()
{
	return _gameTimeDeltaTime;
}

float Time::RealTimeDeltaTime()
{
	return _realTimeDeltaTime;
}

uint Time::GameFrameCount()
{
	return _frameCount;
}

double Time::GameTimeCount()
{
	return _gameTimeTotalTime;
}

double Time::GameTimeInRealTimeCount()
{
	return _realTimeTotalTime;
}

void Time::SetTimeScale(float timeScale)
{
	_timeScale = timeScale;
}

float Time::GetTimeScale()
{
	return _timeScale;
}

void Time::Reset()
{
	// Real Time
	_realTimeDeltaTime = 0;
	_lastFrameTicks = 0;
	_realTimeTotalTime = 0;

	// Game Time
	_gameTimeDeltaTime = 0;
	_frameCount = 0;
	_gameTimeTotalTime = 0;
	_timeScale = 1.0f;

}
