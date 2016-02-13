#!/usr/bin/python2.7

import os
import subprocess
import fileinput
import time

# run generate_html.py
print("generating html documentation from markdown")
os.system("python generate_html.py")

# create readme files in examples with 'try it online' badges with permalinks
examples = "../examples/"
examples_skip = ["multiprocess"]

for dir in os.listdir(examples):
    if not os.path.isdir(examples + dir):
        continue
    
    # skip folders with more than 1 source file
    sources = [s for s in os.listdir(examples + dir) if ".cpp" in s]
    if dir in examples_skip or len(sources) > 1 or len(sources) == 0:
        continue
    
    print("generating readme for example '" + examples + dir + "'")
    
    proc = subprocess.Popen('python send_to_wandbox.py ../doctest/ ' + examples + dir + "/" + sources[0], stdout = subprocess.PIPE)
    url = proc.stdout.read().strip()
    
    if not url.startswith("http"):
        print("skipping current example because of crappy url: '" + url + "'\n")
        continue
    
    readme = open(examples + dir + "/README.md", "w")
    readme.write("[![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](" + url + ")")
    readme.close()
    os.system("git add " + examples + dir + "/README.md")

# update main readme 'try it online' badge permalink
print("updating main readme")
proc = subprocess.Popen('python send_to_wandbox.py ../doctest/ ' + examples + "hello_world/main.cpp", stdout = subprocess.PIPE)
url = proc.stdout.read().strip()

readme_contents = ""
for line in fileinput.input(["../README.md"]):
    if line.startswith("[![Try it online]"):
        readme_contents += "[![Try it online](https://img.shields.io/badge/try%20it-online-orange.svg)](" + url + ")\n"
    else:
        readme_contents += line

readme = open("../README.md", "w")
readme.write(readme_contents)
readme.close()

os.system("git add ../README.md")
