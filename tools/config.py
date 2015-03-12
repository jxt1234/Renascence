MAIN_PROGRAM=[]
gDepends = []
gDirs = []
gSrcFiles = []
gLinks = []
include_Flag = '-Iinclude'

with open('config.txt') as f:
    contents = f.read().split('##########')
    for c in contents:
        lines = c.split('\n')
        lines = [l for l in lines if len(l)>2]
        MAIN_PROGRAM.append(lines[0].split(':')[1])
        gDepends.append(lines[1].split(':')[1])
        gDirs.append([c for c in lines[2].split(':')[1].split(' ') if len(c)>1])
        gSrcFiles.append([c for c in lines[3].split(':')[1].split(' ') if len(c)>1])
        gLinks.append(lines[4].split(':')[1])
    print MAIN_PROGRAM
    print gDirs
    print gSrcFiles
