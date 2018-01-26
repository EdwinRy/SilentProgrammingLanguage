Compiler = gcc
src = src/*.{c,h}
out = bin/SilentVM
bin = bin/*


all:
	$(Compiler) $(src) -Wall -lds -o $(out)

bit32:
	$(Compiler) -m32 $(src) -Wall -o $(out)

install:
	$(Compiler) $(src) -Wall -o $(out)
	sudo cp $(out) /usr/bin/SilentVM

delete:
	sudo rm /usr/bin/SilentVM

gitadd:
	git add $(src)
	git add $(bin)
