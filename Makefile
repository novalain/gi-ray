CC=g++-6 -fopenmp -O3
src=./src/
include=-I./include/
bin=./bin/
bld=./build/
flags= -std=c++14
execfile=./GI-Ray
allsrcfiles=$(src)main.cc

main:
	$(CC) $(flags) $(allsrcfiles) -o $(execfile)

allinone:
	$(CC) $(flags) $(allsrcfiles) $(include) -o $(execfile)

raytracer: $(bld)main.o
	$(CC) $(flags) $(bld)main.o -o $(execfile) #-v -Wall

$(bld)main.o: $(src)main.cc
	$(CC) $(flags) $(include) -o $(bld)main.o -c $(src)main.cc

run:
	$(execfile)

clear:
	rm -rf $(bld)*.o $(execfile) ./results/*.ppm

clearbld:
	rm -rf $(bld)*.o
