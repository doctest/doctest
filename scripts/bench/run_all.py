#!/usr/bin/python2.7

import os
import sys
import json
import subprocess

with open('tests.json') as data_file:    
    data = json.load(data_file)

def runBench(prog):
    result = subprocess.Popen(prog.split(), stdout = subprocess.PIPE).communicate()[0]
    result = result.splitlines()
    for line in result:
        if line.startswith("Time running "):
            return str(line.rsplit(' ', 1)[-1])
    return ""

#print(runBench("python bench.py compile --msvc  --debug --files 200 --tests 0"))

call = 'python ./bench.py'
the_os = 'linux'
if os.name == "nt":
    call = 'python bench.py'
    the_os = 'windows'

f = open('results.txt', 'w')
for test in ['header', 'asserts']:
    print('************** ' + test)
    for framework in ['doctest', 'catch']:
        print('== ' + framework)
        for config in data['compilers'][the_os]:
            #print(config)
            for curr in data[test]:
                if curr[0] == framework or curr[0] == "any":
                    command = call + config + curr[1] + (' --catch' if framework == 'catch' else '')
                    print(command)
                    res = runBench(command)
                    print(res)
                    f.write(res + " ")
            f.write("\n")

f.close()


