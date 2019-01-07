all: test

gen:
	mkdir gen
	../hdf2hepnos.py --input test.h5 --output gen

test: gen main.cpp
	g++ -o myTest main.cpp -I. -lhdf5_cpp -lhdf5

clean:
	rm -f yTest
	rm -rf gen
