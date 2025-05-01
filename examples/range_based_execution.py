#!/usr/bin/python3

import sys
import math
import multiprocessing
import subprocess

if len(sys.argv) < 2:
    print("supply the path to the doctest executable as the first argument!")
    sys.exit(1)

# Get the number of tests in the doctest executable
num_tests = 0

program_with_args = [sys.argv[1], "--dt-count"]
for i in range(2, len(sys.argv)):
    program_with_args.append(sys.argv[i])

result = (
    subprocess.Popen(program_with_args, stdout=subprocess.PIPE)
    .communicate()[0]
    .decode("utf-8")
)
result = result.splitlines(True)

for line in result:
    if line.startswith(
        "[doctest] unskipped test cases passing the current filters:"
    ):
        num_tests = int(line.rsplit(" ", 1)[-1])

# Calculate the ranges
cores = multiprocessing.cpu_count()
l = list(range(num_tests + 1))
n = int(math.ceil(len(l) / cores))
data = [l[i : i + n] for i in range(1, len(l), n)]
data = [(x[0], x[-1]) for x in data]


# Worker function that runs the executable for the given range of tests
def worker(first_last):
    first, last = first_last
    program_with_args = [
        sys.argv[1],
        f"--dt-first={first}",
        f"--dt-last={last}",
    ]
    subprocess.Popen(program_with_args)


# Run the tasks on a pool
if __name__ == "__main__":
    multiprocessing.set_start_method(
        "spawn", force=True
    )  # Ensures compatibility on Windows
    p = multiprocessing.Pool(cores)
    p.map(worker, data)
