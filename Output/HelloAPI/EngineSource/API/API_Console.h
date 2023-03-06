#pragma once
#include <string>
#include "Globals.h"

namespace API
{
	namespace Console
	{
		enum class MessageType
		{
			INFO,
			WARNING,
			ERR
		};

		TO_API void Log(const std::string& text, API::Console::MessageType type = API::Console::MessageType::INFO);
	}
}