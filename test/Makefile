all: test

gen:
	mkdir gen
	hdf2hepnos --input test.h5 --output gen

test: gen main.cpp
	g++ -o myTest -std=c++14 main.cpp -I. -Igen -lhdf5

clean:
	rm -f myTest
	rm -rf gen
