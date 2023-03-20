#pragma once

#include "Globals.h"

static class EngineTime
{
public:
	// Should be Real time and Game Time
	// Frame count since game start
	// time since game start
	// scale at which tme is passing
	// last frame time expressed in seconds
	// last frame time expressed in real time seconds
	// seconds since game start in real time

	static void Start();
	static void StartEngine();

	static void UpdateRealTime();
	static void UpdateGameTime();
	static void UpdateEngineTime();

	/// Get Game deltaTime in seconds.
	static float GameDeltaTime();
	/// Get Real Time deltaTime in seconds.
	static float RealTimeDeltaTime();
	/// Get Engine Time deltaTime in seconds.
	static double EngineTimeDeltaTime();

	/// Get frame count since game start.
	static uint GameFrameCount();

	/// Get game time since game start in seconds.
	static double GameTimeCount();
	/// Get real time since game start in seconds.
	static double GameTimeInRealTimeCount();

	static void SetTimeScale(float timeScale);
	static float GetTimeScale();

	static void Reset();

private:

	// Real Time
	static float _realTimeDeltaTime;
	static uint _lastFrameTicks;
	static double _realTimeTotalTime;
	static uint _startTicks;

	// Game Time
	static float _gameTimeDeltaTime;
	static uint _frameCount;
	static double _gameTimeTotalTime;
	static float _timeScale;

	// Engine Time 
	static double _engineTimeDeltaTime;
	static uint _enginelastFrameTicks;
	static double _engineTimeTotalTime;
	static uint _enginestartTicks;

	
};

