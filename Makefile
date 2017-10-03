Compiler = gcc
Src = SilentVM/src/*.{c,h}
OutputVM = SilentVM/bin/
VMOutput = SilentVM

compilevm:
	$(Compiler) $(Src) -o $(OutputVM)$(VMOutput)
