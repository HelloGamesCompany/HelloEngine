#pragma once
#include "Globals.h"

namespace API
{
	static class TO_API Audio
	{
	public:
		static void Event(const char* eventName);
		static void Event(uint eventID);
	};
}
