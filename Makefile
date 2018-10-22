compiler:
	$(MAKE) -C SilentCompiler
dcompiler:
	$(MAKE) debug -C SilentCompiler
ccompiler:
	$(MAKE) clean -C SilentCompiler

vm:
	$(MAKE) -C SilentVM
dvm:
	$(MAKE) debug -C SilentVM
cvm:
	$(MAKE) clean -C SilentVM

clean:
	$(MAKE) clean -C SilentCompiler
	$(MAKE) clean -C SilentVM
