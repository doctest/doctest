#!/usr/bin/python

import sys
import math
import multiprocessing
import subprocess

if len(sys.argv) < 2:
    print("supply the path to the doctest executable as the first argument!")
    sys.exit(1)

# get the number of tests in the doctest executable
num_tests = 0

program_with_args = [sys.argv[1], "--dt-count=1"]
for i in range(2, len(sys.argv)):
    program_with_args.append(sys.argv[i])

result = subprocess.Popen(program_with_args, stdout = subprocess.PIPE).communicate()[0]
result = result.splitlines(True)
for line in result:
    if line.startswith("[doctest] unskipped test cases passing the current filters:"):
        num_tests = int(line.rsplit(' ', 1)[-1])

# calculate the ranges
cores = multiprocessing.cpu_count()
l = range(num_tests + 1)
n = int(math.ceil(float(len( l )) / cores))
data = [l[i : i + n] for i in range(1, len( l ), n)]
data = tuple([[x[0], x[-1]] for x in data])

# for 8 cores and 100 tests the ranges will look like this
# ([1, 13], [14, 26], [27, 39], [40, 52], [53, 65], [66, 78], [79, 91], [92, 100])

# the worker callback that runs the executable for the given range of tests
def worker((first, last)):
    program_with_args = [sys.argv[1], "--dt-first=" + str(first), "--dt-last=" + str(last)]
    subprocess.Popen(program_with_args)

# run the tasks on a pool
if __name__ == '__main__':
    p = multiprocessing.Pool(cores)
    p.map(worker, data)
