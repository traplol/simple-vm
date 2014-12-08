all:
	@echo "Making vm..."
	@cd vm; make; make vm; make tests
	@echo "Done"
	@echo "Making assembler..."
	@cd assembler; make
	@echo "Done"

clean:
	@echo "Cleaning vm..."
	@cd vm; make clean
	@echo "Done"
	@echo "Making assembler..."
	@cd assembler; make clean
	@echo "Done"


