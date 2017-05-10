#!/usr/bin/python2.7

import os
import sys
#import pprint
import json

with open('tests.json') as data_file:    
    data = json.load(data_file)

#pprint.pprint(data)

f = open('results.txt', 'w')
if os.name == "nt":
    call = 'bench.py'
    #f.write('header cost')
    for test in ['header', 'asserts']:
        print('************** ' + test)
        for framework in ['doctest', 'catch']:
            print('== ' + framework)
            for config in data['compilers']['windows']:
                #print(config)
                for curr in data[test]:
                    if curr[0] == framework or curr[0] == "any":
                        print(call + config + curr[1] + (' --catch' if framework == 'catch' else ''))


f.close()


