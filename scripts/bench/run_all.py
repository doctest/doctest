#!/usr/bin/python3

import os
import sys
if sys.version_info[0] < 3: raise Exception("Python 3 or a more recent version is required.")
import json
import subprocess

average_num_times = 3
max_accum_time    = 60  # don't take too long on a test - stop averaging if time exceeds some amount of seconds

with open('tests.json') as data_file:    
    data = json.load(data_file)

def runBench(prog):
    result = subprocess.Popen(prog.split(), stdout = subprocess.PIPE).communicate()[0]
    result = result.splitlines()
    for line in result:
        line = line.decode("utf-8")
        if line.startswith("Time running "):
            return str(line.rsplit(' ', 1)[-1])
    return ""

call = 'python ./bench.py'
the_os = 'linux'
if os.name == "nt":
    call = 'python bench.py'
    the_os = 'windows'

f = open('results.txt', 'w')
for test in ['header', 'asserts', 'runtime']:
    print(  '\n************** ' + test + '\n')
    f.write('\n************** ' + test + '\n')
    f.flush()
    for framework in ['doctest', 'catch']:
        print(  '== ' + framework + '\n')
        f.write('== ' + framework + '\n')
        f.flush()
        for config in data['compilers'][the_os]:
            for curr in data[test][1]:
                if curr[0] == framework or curr[0] == "any":
                    command = call + data[test][0] + config + curr[1] + (' --catch' if framework == 'catch' else '')
                    print(command)
                    
                    accum = float(0)
                    num_times = 0
                    for i in range(0, average_num_times):
                        res = float(runBench(command))
                        print(res)
                        accum += res
                        num_times += 1
                        
                        if accum > max_accum_time:
                            break
                    
                    average = "{:7.2f}".format(round(accum / num_times, 2))
                    print("AVERAGE: " + average)
                    f.write(average + " | ")
                    f.flush()
            f.write("\n")
            f.flush()

f.close()


