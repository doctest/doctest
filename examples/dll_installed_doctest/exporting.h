#pragma once

#ifdef _EXPORT
#ifdef _MSC_VER
#define DLL_API __declspec(dllexport)
#elif defined __GNUC__
#define DLL_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define DLL_API __declspec(dllimport)
#elif defined __GNUC__
#define DLL_API __attribute__((visibility("hidden")))
#endif
#endif