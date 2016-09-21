#!/usr/bin/python2.7

# STUFF THAT DOESN'T WORK:
# - before doing a list with '-' make sure to put an empty line before it
# - external links with '.md' in them get broken - gets replaced with '.html'
# - anchors that work in github don't work in the generated html so all interlinking with anchors doesn't work

import os

html_dir = "../doc/html_generated/"
md_dir = "../doc/markdown/"

filelist = [f for f in os.listdir(html_dir) if f.endswith(".html")]
for f in filelist:
    os.remove(html_dir + f)

for filename in os.listdir(md_dir):
    if filename[-2:] == "md":
        md = open(md_dir + filename, "r")
        md_contents = md.read()
        md.close()
        html = open(html_dir + filename[:-3] + ".html", "w")
        html.write('<!DOCTYPE html>\n')
        html.write('<html>\n')
        html.write('<title>' + filename[:-3] + '</title>\n')
        html.write('<xmp theme="united" style="display:none;">\n\n')
        md_contents = md_contents.replace(".md", ".html")
        md_contents = md_contents.replace("```c++", "```")
        html.write(md_contents)
        html.write('\n\n</xmp>\n')
        html.write('<script src="strapdown.js/strapdown.js"></script>\n')
        html.write('</html>\n')
        html.close()
