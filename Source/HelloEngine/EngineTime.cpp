#include "Headers.h"

// Real Time
float EngineTime::_realTimeDeltaTime = 0;
uint EngineTime::_lastFrameTicks = 0;
double EngineTime::_realTimeTotalTime = 0;
uint EngineTime::_startTicks = 0;

// Game Time
float EngineTime::_gameTimeDeltaTime = 0;
uint EngineTime::_frameCount = 0;
double EngineTime::_gameTimeTotalTime = 0;
float EngineTime::_timeScale = 1.0f;

void EngineTime::Start()
{
	_startTicks = SDL_GetTicks();
	_lastFrameTicks = 0;
}

void EngineTime::UpdateRealTime()
{
	// Get real time in seconds (SDL_GetTicks() returns in miliseconds)
	_realTimeDeltaTime = (SDL_GetTicks() - _lastFrameTicks - _startTicks) * 0.001f;
	_lastFrameTicks = SDL_GetTicks() - _startTicks;
	_realTimeTotalTime += _realTimeDeltaTime;

}

void EngineTime::UpdateGameTime()
{
	_gameTimeDeltaTime = RealTimeDeltaTime() * _timeScale;
	_gameTimeTotalTime += _gameTimeDeltaTime;
	// Add frame count
	_frameCount++;
}

float EngineTime::GameDeltaTime()
{
	return _gameTimeDeltaTime;
}

float EngineTime::RealTimeDeltaTime()
{
	return _realTimeDeltaTime;
}

uint EngineTime::GameFrameCount()
{
	return _frameCount;
}

double EngineTime::GameTimeCount()
{
	return _gameTimeTotalTime;
}

double EngineTime::GameTimeInRealTimeCount()
{
	return _realTimeTotalTime;
}

void EngineTime::SetTimeScale(float timeScale)
{
	_timeScale = timeScale;
}

float EngineTime::GetTimeScale()
{
	return _timeScale;
}

void EngineTime::Reset()
{
	// Real Time
	_realTimeDeltaTime = 0;
	_lastFrameTicks = 0;
	_realTimeTotalTime = 0;
	_startTicks = 0;

	// Game Time
	_gameTimeDeltaTime = 0;
	_frameCount = 0;
	_gameTimeTotalTime = 0;
	_timeScale = 1.0f;

}
