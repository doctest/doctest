#!/usr/bin/python2.7

with_gcc = 0
is_debug = 1

numFiles = 100

with_doctest = 1

with_implement      = 1
with_header         = 1
with_num_tests      = 25
with_num_assertions = 10
doctest_disable     = 0

the_folder = "project"

GCC_cmake_generator     = '"MinGW Makefiles"'
MSVC_cmake_generator    = '"Visual Studio 14 Win64"' # MSVC 2015

# ==============================================================================
# ==============================================================================
# ==============================================================================

import os
import sys
import random
import string
import multiprocessing
from datetime import datetime
import shutil
from time import sleep

# clean and make the folder
if os.path.exists(the_folder):
    shutil.rmtree(the_folder)

sleep(2) # or the script might fail...

if not os.path.exists(the_folder):
    os.makedirs(the_folder)

# make the source files
for i in range(0, numFiles):
    f = open(the_folder + '/' + str(i) + '.cpp', 'w')
    if with_header:
        if with_doctest:
            if doctest_disable:
                f.write('#define DOCTEST_CONFIG_DISABLE\n')
            f.write('#include "doctest.h"\n\n')
        else:
            f.write('#include "catch.hpp"\n\n')
        for t in range(0, with_num_tests):
            f.write('TEST_CASE("") {\n')
            for a in range(0, with_num_assertions):
                f.write('    int a' + str(a) + ' = 5;\n')
                f.write('    int b' + str(a) + ' = 6;\n')
                f.write('    CHECK(a' + str(a) + ' == b' + str(a) + ');\n')
            f.write('}\n\n')
    f.write('int f' + str(i) + '() { return ' + str(i) + '; }\n\n')
    f.close()

# the main file
f = open(the_folder + '/main.cpp', 'w')
if with_implement:
    if with_doctest:
        if doctest_disable:
            f.write('#define DOCTEST_CONFIG_DISABLE\n')
        f.write('#define DOCTEST_CONFIG_IMPLEMENT\n')
        f.write('#include "doctest.h"\n\n')
    else:
        f.write('#define CATCH_CONFIG_RUNNER\n')
        f.write('#include "catch.hpp"\n\n')
f.write('int main(int argc, char** argv) {\n')
if with_implement:
    if with_doctest:    f.write('    int res = doctest::Context(argc, argv).run();\n')
    else:               f.write('    int res = Catch::Session().run(argc, argv);\n')
else:
    f.write('    int res = 0;\n')
for i in range(0, numFiles):
    f.write('    int f' + str(i) + '(); res += f' + str(i) + '();\n')
f.write('    return res;\n}\n')
f.close()

# the cmake file
f = open(the_folder + '/CMakeLists.txt', 'w')
f.write('cmake_minimum_required(VERSION 2.8)\n\n')
f.write('project(bench)\n\n')
if with_doctest:    f.write('include_directories("../../../doctest/")\n\n')
else:               f.write('include_directories("../catch/single_include/")\n\n')
f.write('add_executable(bench main.cpp\n')
for i in range(0, numFiles):
    f.write('    ' + str(i) + '.cpp\n')
f.write(')\n')
f.close()

# invoke cmake
os.chdir(the_folder);

if with_gcc:
    cmake_build_type = 'Release'
    if is_debug:
        cmake_build_type = 'Debug'
    # twice because MinGW-w64 fails the first time - don't know why
    os.system('cmake . -G ' + GCC_cmake_generator + ' -DCMAKE_BUILD_TYPE=' + cmake_build_type);
    os.system('cmake . -G ' + GCC_cmake_generator + ' -DCMAKE_BUILD_TYPE=' + cmake_build_type);
else:
    os.system('cmake . -G ' + MSVC_cmake_generator);

the_config = ''
if with_gcc == False:
    if is_debug:
        the_config = ' --config Debug'
    else:
        the_config = ' --config Release'

# build it
start = datetime.now()
os.system('cmake --build .' + the_config)
end = datetime.now()

print("Time for compiling (+ linking): " + str(end - start))

os.chdir("../");














