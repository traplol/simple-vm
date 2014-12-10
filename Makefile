all: | bindir
	@echo "Making vm..."
	@cd vm && make && cp bin/vm ../bin/
	@echo "Copying vm to bin/"
	@echo "Done"
	@echo "Making assembler..."
	@cd assembler && make && cp bin/assembler ../bin/
	@echo "Copying assembler to bin/"
	@echo "Done"

clean: | bindir
	@echo "Cleaning vm..."
	@cd vm && make clean
	@echo "Done"
	@echo "Cleaning assembler..."
	@cd assembler && make clean
	@echo "Done"
	@echo "Cleaning bin/"
	cd bin/; rm -f vm; rm -f assembler; rm -f *.bin
	@echo "Done"

bindir:
	@mkdir -p bin
