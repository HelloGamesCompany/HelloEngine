#pragma once

#ifdef HELLO_ENGINE_EXPORTS
#define HELLO_ENGINE_API __declspec(dllexport)
#define HELLO_ENGINE_API_C extern "C" __declspec(dllexport)
#else
#define HELLO_ENGINE_API __declspec(dllimport)
#define HELLO_ENGINE_API_C 
#endif