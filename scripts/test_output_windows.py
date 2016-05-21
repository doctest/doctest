#!/usr/bin/python2.7

import os
import sys

os.chdir("../")

if not os.path.exists("build"): os.makedirs("build")
if not os.path.exists("build-mingw"): os.makedirs("build-mingw")

res = 0

os.chdir("build")
res = res + os.system("cmake .. -G \"Visual Studio 14 2015 Win64\"");
res = res + os.system("cmake .. -DTEST_MODE=COLLECT");
res = res + os.system("cmake --build .");
res = res + os.system("ctest -C Debug --output-on-failure");
res = res + os.system("cmake .. -DTEST_MODE=COMPARE");
res = res + os.system("ctest -C Debug --output-on-failure");
os.chdir("../")

os.chdir("build-mingw")
res = res + os.system("cmake .. -G \"MinGW Makefiles\" -DCMAKE_BUILD_TYPE=Debug");
res = res + os.system("cmake .. -G \"MinGW Makefiles\"");
res = res + os.system("cmake .. -DTEST_MODE=COMPARE");
res = res + os.system("cmake --build .");
res = res + os.system("ctest --output-on-failure");
os.chdir("../")

os.chdir("scripts")

if res != 0:
    print "!!! OMGOMGOMG SOMETHING FAILED!\n"
    print "!!! OMGOMGOMG SOMETHING FAILED!\n"
    print "!!! OMGOMGOMG SOMETHING FAILED!\n"
    sys.exit(1)
