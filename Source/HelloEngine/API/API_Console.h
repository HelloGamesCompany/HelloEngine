#pragma once
#include <string>
#include "Globals.h"

#include "API_Vector3.h"
#include "API_Vector2.h"

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
		//TO_API void Log(const API::API_Vector2& vec, API::Console::MessageType type = API::Console::MessageType::INFO);

		TO_API void Log(const API::API_Vector3& vec, API::Console::MessageType type = API::Console::MessageType::INFO);

		TO_API void Log(const std::string& text, API::Console::MessageType type = API::Console::MessageType::INFO);
	}
}