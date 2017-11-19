compiler = gcc
src = src/*.{c,h}
##src = src/SilentCompiler.c src/SilentCompiler.h
out = bin/SilentC
all:
	$(compiler) $(src) -o $(out)

gitadd:
	git add $(src)
	git add $(out)
	git add bin/*
	git add Makefile
