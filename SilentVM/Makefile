Compiler = gcc
src = src/*.{c,h}
out = bin/SilentVM
bin = bin/*

all:
	$(Compiler) $(src) -Wall -o $(out)

gitadd:
	git add $(src)
	git add $(bin)
