all:
	cd vm; make
	cd assembler; make

clean:
	cd vm; make clean
	cd assembler; make clean


