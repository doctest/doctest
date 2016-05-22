#pragma once

// taken from https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
#else
  #ifdef BUILDING_DLL
    #if __GNUC__ >= 4
      #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #else
      #define DLL_PUBLIC
    #endif
  #else
    #define DLL_PUBLIC
  #endif
#endif

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>

TEST_CASE("shared") {
    printf("I am a test in a header used by the executable and the dll!\n");
}

DLL_PUBLIC int call_tests_from_dll(int argc, char** argv);
