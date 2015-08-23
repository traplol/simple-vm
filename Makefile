all: bin vm assembler

.PHONY: vm assembler bindings clean

vm:
	@echo "Making vm..."
	@cd vm && make && cp bin/vm ../bin/
	@echo "Copying vm to bin/"
	@echo "Done"

assembler:
	@echo "Making assembler..."
	@cd vm && make archive
	@cd assembler && make && cp bin/assembler ../bin/
	@echo "Copying assembler to bin/"
	@echo "Done"

clean: bin lib
	@echo "Cleaning vm..."
	@cd vm && make clean
	@echo "Done"
	@echo "Cleaning assembler..."
	@cd assembler && make clean
	@echo "Done"
	@echo "Cleaning bin/"
	@cd bin && rm -f vm assembler *.bin *.dump
	@cd lib && rm -f *.so *.a
	@echo "Done"

bindings: lib
	@cd vm && make bindings && cp lib/* ../lib/
	@cd assembler && make bindings && cp lib/* ../lib/

bin:
	@mkdir -p bin

lib:
	@mkdir -p lib
