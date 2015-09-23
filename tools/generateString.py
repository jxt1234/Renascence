#!/usr/bin/python

def analisis(lines):
    result = {}
    for l in lines:
        if len(l)<2:
            continue
        words = l.split(' ')
        if len(words)<2:
            continue
        result[words[0]] = words[1]
    return result

def main():
    import sys
    gHead = "GPStrings.h"
    gCpp = "GPStrings.cpp"
    dirs = "res/string/"
    if len(sys.argv)>1:
        dirs = sys.argv[1]
    print dirs
    hContents = '#ifndef INCLUDE_GPSTRING_H\n#define INCLUDE_GPSTRING_H\n'
    cContents = '#include \"'+gHead+'\"\n'
    hContents += 'namespace GPStrings{\n'
    cContents += 'namespace GPStrings{\n'
    import os
    for filename in os.listdir(dirs):
        classname = filename.split('.')[0]
        print filename, classname
        with open(dirs+filename) as f:
            lists = analisis(f.read().split('\n'))
            for k,v in lists.iteritems():
                print k, v
                hContents += 'extern const char* ' + classname + '_' + k + ';\n'
                cContents += 'const char* '+classname + '_' + k + ' = \"' + v + '\";\n'
    hContents+='}\n'
    hContents+='#endif\n'
    cContents+='}\n'
    with open('include/'+gHead, 'w') as f:
        f.write(hContents)
    with open('src/'+gCpp, 'w') as f:
        f.write(cContents)
if __name__ == '__main__':
    main()
