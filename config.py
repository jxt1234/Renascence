MAIN_PROGRAM=['libGP.so', 'test.out', 'gpsuit.out', 'example.out']
gDepends = ['', 'libGP.so', 'libGP.so', 'libGP.so']
gDirs = [['src'], ['test'], ['GPSuit'], []]
gSrcFiles = [[], [], [], ['simple_main.cpp']]
gLinks = [' -lpthread -ldl', ' ./libGP.so -ldl -lpthread ', ' ./libGP.so', './libGP.so -ldl -lpthread']
include_Flag = '-Iinclude'
