#!/usr/bin/python
import os
if __name__ == '__main__':
    cmd = 'find include src -name \"*.h\"'
    hFiles = set(filter(lambda x:len(x)>1, os.popen(cmd).read().split('\n')))
    cmd = 'find include src -name \"*.cpp\"'
    cFiles = set(filter(lambda x:len(x)>1, os.popen(cmd).read().split('\n')))
    files = hFiles | cFiles
    licensecontent = open('res/license').read()
    for f in files:
        contents = ''
        with open(f) as _f:
            contents = _f.read()
        if (contents.find('Licensed under the Apache License, Version 2.0')!=-1):
            continue
        with open(f, 'w') as _f:
            _f.write(licensecontent)
            _f.write(contents)
