CC=g++ -std=c++14
src=./src/
bin=./bin/
build=./build/
execfile=GI-Ray

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: allinone #raytracer

allinone: $(src)main.cc $(src)camera.cc
	$(CC) $(src)main.cc $(src)camera.cc -o $(bin)$(execfile) -Wall

raytracer: main.o
	$(CC) $(build)main.o $(build)camera.o -o $(bin)$(execfile) -Wall

main.o: $(src)main.cc camera.o
	$(CC) -c $(src)main.cc -o $(build)main.o

camera.o: $(src)camera.cc
	$(CC) -c $(src)camera.cc -o $(build)camera.o

run:
	./bin/GI-Ray

clear:
	rm -rf $(build)*.o $(bin)$(execfile)
