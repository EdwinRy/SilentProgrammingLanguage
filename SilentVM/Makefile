compiler = gcc
source = src/*.c
output = bin/SilentVM
standard = -std=c99
flags = -Wall

all:
	$(compiler) $(standard) $(flags) $(source) -o $(output)

winLib:
	gcc -shared -o lib/silentLib.dll lib/silentLib.c

linLib:
	gcc -shared -fpic lib/silentLib.c -o lib/silentLib.so