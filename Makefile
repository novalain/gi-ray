CC=g++ -std=c++14
src=./src/
bin=./bin/

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: Raytracer

Raytracer: $(src)main.cpp
	$(CC) $(src)main.cpp -o $(bin)GI-Ray -Wall

main.o: main.cpp
	$(CC) -c main.cpp

run:
	./bin/GI-Ray

#clear:
#	rm -rf *.o
