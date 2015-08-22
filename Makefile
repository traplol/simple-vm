all: bin vm assembler

.PHONY: vm assembler clean

vm:
	@echo "Making vm..."
	@cd vm && make && cp bin/vm ../bin/
	@echo "Copying vm to bin/"
	@echo "Done"

assembler:
	@echo "Making assembler..."
	@cd assembler && make && cp bin/assembler ../bin/
	@echo "Copying assembler to bin/"
	@echo "Done"

clean: bin
	@echo "Cleaning vm..."
	@cd vm && make clean
	@echo "Done"
	@echo "Cleaning assembler..."
	@cd assembler && make clean
	@echo "Done"
	@echo "Cleaning bin/"
	cd bin/; rm -f vm assembler *.bin *.dump
	@echo "Done"

bin:
	@mkdir -p bin
