#!/usr/bin/python2.7

import os

# run generate_html.py

os.system("python generate_html.py")

# create readme files in examples with 'try it online' badges with permalinks

for root, dirs, files in os.walk("../examples/"):
    for dir in dirs:
        readme = open(root + dir + "README.md", "w")
        readme.write("")
        readme.close()
        os.system("git add " + root + dir + "README.md")


# update main readme 'try it online' badge permalink

