#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "External/SDL/include/SDL.h"

class Timer
{
private:

	uint mStartTicks = 0;
	uint mElapsedTicks = 0;
	float mDeltaTime = 0;
	float mTimeScale = 0;
	float lastTime = 0;

public:

	Timer()
	{
		Reset();
		mTimeScale = 1.0f;
	}

	~Timer()
	{
	}

	void Reset()
	{
		mStartTicks = SDL_GetTicks();
		mElapsedTicks = 0;
		mDeltaTime = 0.0f;
	}

	float getDeltaTime()
	{
		Update();
		return mDeltaTime;
	}

	void setTimeScale(float t)
	{
		mTimeScale = t;
	}

	float getTimeScale()
	{
		return mTimeScale;
	}

	float getExecuteTime(bool second = false)
	{
		if (second)
		{
			return  (SDL_GetTicks() - (SDL_GetTicks() % 1000)) * 0.001f;
		}

		return SDL_GetTicks();
	}

	float Update()
	{
		// frame2frame = tiempo que tarda entre ultimo frame hasta este frame en ms
		float frameTime = SDL_GetTicks() - lastTime;
		// Reseteamos el ultimo frame para que sea este
		lastTime = SDL_GetTicks();
		// mElapsedTicks = tiempo que ha pasado desde ultimo reset en ms
		mElapsedTicks = SDL_GetTicks() - mStartTicks;
		// mElapsedTicks = tiempo que ha pasado desde ultimo reset en segundo
		mDeltaTime = mElapsedTicks * 0.001f;
		// retornar frame2frame en segundo
		return (frameTime * 0.001f);
	}
};

#endif