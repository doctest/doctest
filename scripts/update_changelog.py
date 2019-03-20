#!/usr/bin/python2.7

import os

# the version of the release
with open("version.txt") as f: version = f.read()

# update changelog
print("updating changelog")
os.chdir("../")
os.system("github_changelog_generator --future-release " + version)
os.system("git add CHANGELOG.md")
os.chdir("scripts")
