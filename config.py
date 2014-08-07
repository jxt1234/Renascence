MAIN_PROGRAM=['libGP.so', 'test.out', 'gpsuit.out']
gDepends = ['', 'libGP.so', 'libGP.so']
gDirs = [['src'], ['test'], ['GPSuit']]
gSrcFiles = [[], [], []]
gLinks = [' -lpthread -ldl', ' ./libGP.so -ldl -lpthread ', ' ./libGP.so']
include_Flag = '-Iinclude'
