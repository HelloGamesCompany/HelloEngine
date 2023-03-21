#pragma once
#include "Globals.h"

namespace API
{
	static class TO_API Audio
	{
	public:
		void Event(const char* eventName);
		void Event(uint eventID);
	};
}
