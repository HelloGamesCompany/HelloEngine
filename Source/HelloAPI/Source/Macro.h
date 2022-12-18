#pragma once

#ifdef HELLO_ENGINE_EXPORTS
#define HELLO_ENGINE_API __declspec(dllexport)
#else
#define HELLO_ENGINE_API __declspec(dllimport)
#endif