CC=g++ -std=c++14
src=./src/
bin=./bin/

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: Raytracer

Raytracer: $(src)main.cc
	$(CC) $(src)main.cc -o $(bin)GI-Ray -Wall

main.o: main.cc
	$(CC) -c main.cc

run:
	./bin/GI-Ray

#clear:
#	rm -rf *.o
