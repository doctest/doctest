#!/usr/bin/python3

import os
import sys
if sys.version_info[0] < 3: raise Exception("Python 3 or a more recent version is required.")
import pprint
import argparse
import urllib.request
from datetime import datetime
import shutil
from time import sleep

# ==============================================================================
# == ARGUMENTS =================================================================
# ==============================================================================

def addCommonFlags(parser):
    parser.add_argument("compiler",     choices=['msvc', 'gcc', 'clang'], default='msvc', help = "compiler to use")
    parser.add_argument("--debug",      action = "store_true",  help = "build in debug")
    parser.add_argument("--catch",      action = "store_true",  help = "use Catch instead of doctest")
    parser.add_argument("--disabled",   action = "store_true",  help = "DOCTEST_CONFIG_DISABLE / CATCH_CONFIG_DISABLE")
    parser.add_argument("--fast",       action = "store_true",  help = "define the doctest/Catch fast config identifier")
    parser.add_argument("--files",      type=int, default=1,    help = "number of source files (besides the implementation)")
    parser.add_argument("--tests",      type=int, default=1,    help = "number of test cases per source file")
    parser.add_argument("--checks",     type=int, default=1,    help = "number of asserts per test case")
    parser.add_argument("--asserts",    choices=['normal', 'binary'], default="normal",
                                                                help = "<doctest> type of assert used - Catch: only normal")

parser = argparse.ArgumentParser()
subparsers = parser.add_subparsers()
parser_c = subparsers.add_parser('compile', help='benchmark compile times')
addCommonFlags(parser_c)
parser_c.add_argument("--implement",    action = "store_true",  help = "implement the framework test runner")
parser_c.add_argument("--header",       action = "store_true",  help = "include the framework header everywhere")
parser_r = subparsers.add_parser('runtime', help='benchmark runtime')
addCommonFlags(parser_r)
parser_r.add_argument("--loop-iters",   type=int, default=1000, help = "loop N times all asserts in each test case")
parser_r.add_argument("--info",         action = "store_true",  help = "log the loop variable with INFO()")

def compile(args): args.compile = True; args.runtime = False
def runtime(args): args.compile = False; args.runtime = True
parser_c.set_defaults(func=compile)
parser_r.set_defaults(func=runtime)
args = parser.parse_args()
args.func(args)

print("== PASSED OPTIONS TO BENCHMARK SCRIPT:")
pprint.pprint(vars(args), width = 1)

# ==============================================================================
# == SETUP ENVIRONMENT =========================================================
# ==============================================================================

# catch version
catch_ver = "2.3.0"
catch_header = "catch." + catch_ver + ".hpp"

# get the catch header
if not os.path.exists("catch." + catch_ver + ".hpp"):
    urllib.request.urlretrieve("https://github.com/catchorg/Catch2/releases/download/v" + catch_ver + "/catch.hpp", catch_header)

# folder with generated code
the_folder = 'project'

# delete the folder
if os.path.exists(the_folder):
    shutil.rmtree(the_folder)

# wait a bit or the script might fail...
sleep(2)

# create the folder
if not os.path.exists(the_folder):
    os.makedirs(the_folder)

# enter folder
os.chdir(the_folder);

# ==============================================================================
# == DO STUFF ==================================================================
# ==============================================================================

# setup defines used
defines = ""
if args.catch and args.disabled:
    defines += "#define CATCH_CONFIG_DISABLE\n"
if not args.catch and args.disabled:
    defines += "#define DOCTEST_CONFIG_DISABLE\n"
if args.catch and args.fast:
    defines += "#define CATCH_CONFIG_FAST_COMPILE\n"
if not args.catch and args.fast:
    defines += "#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS\n"

define_implement = "#define DOCTEST_CONFIG_IMPLEMENT\n"
if args.catch:
    define_implement = "#define CATCH_CONFIG_RUNNER\n"

# setup the macros used
macro = "    CHECK(a == b);\n"
if args.runtime:
    macro = "    CHECK(i == i);\n"
if not args.catch and args.asserts == "binary":
    macro = "    CHECK_EQ(a, b);\n"

# setup the header used
include = '#include "doctest.h"\n'
if args.catch:
    include = '#include "' + catch_header + '"\n'

# ==============================================================================
# == GENERATE SOURCE CODE ======================================================
# ==============================================================================

# make the source files
for i in range(0, args.files):
    f = open(str(i) + '.cpp', 'w')
    if args.runtime or args.header:
        f.write(defines)
        f.write(include)
        for t in range(0, args.tests):
            f.write('TEST_CASE("") {\n')
            f.write('    int a = 5;\n')
            f.write('    int b = 5;\n')
            if args.runtime and args.loop_iters > 0:
                f.write('    for(int i = 0; i < ' + str(args.loop_iters) + '; ++i) {\n')
            if args.runtime and args.info:
                f.write('        INFO(i);\n')
            for a in range(0, args.checks):
                if args.runtime and args.loop_iters > 0:
                    f.write('    ')
                f.write(macro)
            if args.runtime and args.loop_iters > 0:
                f.write('    }\n')
            f.write('}\n\n')
    f.write('int f' + str(i) + '() { return ' + str(i) + '; }\n\n')
    f.close()

# the main file
f = open('main.cpp', 'w')
if args.runtime or args.implement or args.header:
    f.write(defines)
    f.write(define_implement)
    f.write(include)
f.write('int main(int argc, char** argv) {\n')
if args.runtime or args.implement or args.header:
    if not args.catch:  f.write('    int res = doctest::Context(argc, argv).run();\n')
    else:               f.write('    int res = Catch::Session().run(argc, argv);\n')
else:
    f.write('    int res = 0;\n')
for i in range(0, args.files):
    f.write('    int f' + str(i) + '(); res += f' + str(i) + '();\n')
f.write('    return res;\n}\n')
f.close()

# the cmake file
f = open('CMakeLists.txt', 'w')
f.write('cmake_minimum_required(VERSION 2.8)\n\n')
f.write('project(bench)\n\n')
f.write('if(NOT MSVC)\n')
f.write('set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")\n')
f.write('endif()\n\n')
if not args.catch:  f.write('include_directories("../../../doctest/")\n\n')
else:               f.write('include_directories("../")\n\n')
f.write('add_executable(bench main.cpp\n')
for i in range(0, args.files):
    f.write('    ' + str(i) + '.cpp\n')
f.write(')\n')
f.close()

# ==============================================================================
# == INVOKE CMAKE ==============================================================
# ==============================================================================

compiler = ""
if args.compiler == 'clang':
    compiler = " -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS=-w"
if args.compiler == 'gcc':
    compiler = " -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS=-w"

# setup cmake command
cmake_command = 'cmake . -G "Visual Studio 15 Win64"' # MSVC 2017
if args.compiler != 'msvc':
    cmake_command = 'cmake . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=' + ('Debug' if args.debug else 'Release')
if os.name != "nt":
    cmake_command = 'cmake .                      -DCMAKE_BUILD_TYPE=' + ('Debug' if args.debug else 'Release')

os.system(cmake_command + compiler)

# ==============================================================================
# == BUILD PROJECT =============================================================
# ==============================================================================

the_config = ''
if args.compiler == 'msvc':
    if args.debug:  the_config = ' --config Debug'
    else:           the_config = ' --config Release'

# build it
start = datetime.now()
os.system('cmake --build .' + the_config)
end = datetime.now()

if not args.runtime:
    print("Time running compiler (+ linker) in seconds: " + str((end - start).total_seconds()))

# ==============================================================================
# == RUN PROJECT ===============================================================
# ==============================================================================

if args.runtime:
    start = datetime.now()
    if args.compiler == 'msvc':
        os.system(('Debug' if args.debug else 'Release') + '\\bench.exe')
    elif os.name == "nt":
        os.system('bench.exe')
    else:
        os.system('./bench')
    end = datetime.now()

    print("Time running the tests in seconds: " + str((end - start).total_seconds()))

# leave folder
os.chdir("../");














