all:
	cd vm; make; make vm; make tests
	cd assembler; make

clean:
	cd vm; make clean
	cd assembler; make clean


