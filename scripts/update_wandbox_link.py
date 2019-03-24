#!/usr/bin/python2.7

import subprocess
import fileinput

# update main readme 'try it online' badge permalink
print("updating main readme with up-to-date wandbox link")
proc = subprocess.Popen('python send_to_wandbox.py ../doctest/ ' + "../scripts/hello_world.cpp", stdout = subprocess.PIPE)
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
