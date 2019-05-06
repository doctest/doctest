#!/usr/bin/python2.7

import os
import fileinput

# the version of the release
with open("version.txt") as f: version = f.read()

def getVersionTuple(v):
    return tuple(map(int, (v.split("."))))

version_major = str(getVersionTuple(version)[0])
version_minor = str(getVersionTuple(version)[1])
version_patch = str(getVersionTuple(version)[2])

# update version in the header file
print("updating the version in the header file")
doctest_contents = ""
for line in fileinput.input(["../doctest/parts/doctest_fwd.h"]):
    if line.startswith("#define DOCTEST_VERSION_MAJOR "):
        doctest_contents += "#define DOCTEST_VERSION_MAJOR " + version_major + "\n"
    elif line.startswith("#define DOCTEST_VERSION_MINOR "):
        doctest_contents += "#define DOCTEST_VERSION_MINOR " + version_minor + "\n"
    elif line.startswith("#define DOCTEST_VERSION_PATCH "):
        doctest_contents += "#define DOCTEST_VERSION_PATCH " + version_patch + "\n"
    elif line.startswith("#define DOCTEST_VERSION_STR "):
        doctest_contents += "#define DOCTEST_VERSION_STR \"" + version + "\"\n"
    else:
        doctest_contents += line

readme = open("../doctest/parts/doctest_fwd.h", "w")
readme.write(doctest_contents)
readme.close()

# update meson file with version
meson_contents = ""
for line in fileinput.input(["../meson.build"]):
    if line.startswith("project('doctest'"):
        meson_contents += "project('doctest', ['cpp'], version: '" + version + "', meson_version:'>=0.50')\n"
    else:
        meson_contents += line

meson = open("../meson.build", "w")
meson.write(meson_contents)
meson.close()

# run generate_html.py
print("generating html documentation from markdown")
os.system("python generate_html.py")
