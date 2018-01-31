compiler = gcc
src = src/*.{c,h}
##src = src/SilentCompiler.c src/SilentCompiler.h
out = bin/SilentC
all:
	$(compiler) $(src) -o $(out)

cpp:
	g++ -Wall src_cpp/*.cpp -o bin/SilentC2
	
bit32:
	$(compiler) -m32 $(src) -o $(out)

