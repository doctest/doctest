#!/usr/bin/python

import sys
import math
import multiprocessing
import subprocess

if len(sys.argv) != 2:
    print("supply the path to the doctest executable as the first argument!")
    sys.exit(0)

# get the number of tests in the doctest executable
num_tests = int(subprocess.Popen([sys.argv[1], "-doctest_count=true"], stdout = subprocess.PIPE).communicate()[0])

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
    subprocess.Popen([sys.argv[1], "-doctest_first=%s -doctest_last=%s" % (first, last)])

# run the tasks on a pool
if __name__ == '__main__':
    p = multiprocessing.Pool(cores)
    p.map(worker, data)
