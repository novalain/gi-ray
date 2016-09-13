CC=g++ -std=c++14
src=./src/
include=./include/
bin=./bin/
build=./build/
execfile=GI-Ray

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: allinone #raytracer

allinone: #$(src)main.cc $(src)camera.cc $(src)vertex.cc $(src)triangle.cc $(src)ray.cc
	$(CC) $(src)main.cc $(src)camera.cc $(src)vertex.cc $(src)triangle.cc $(src)ray.cc -o $(bin)$(execfile) -I$(include) -Wall

#raytracer: main.o
#	$(CC) $(build)main.o $(build)camera.o $(build)vertex.o $(build)triangle.o $(build)ray.o -o $(bin)$(execfile) -Wall

#main.o: $(src)main.cc camera.o ray.o
#	$(CC) -c $(src)main.cc -o $(build)main.o

#camera.o: $(src)camera.cc
#	$(CC) -c $(src)camera.cc -o $(build)camera.o

#vertex.o: $(src)vertex.cc
#	$(CC) -c $(src)vertex.cc -o $(build)vertex.o

#triangle.o: $(src)triangle.cc vertex.o
#	$(CC) -c $(src)triangle.cc -o $(build)triangle.o

#ray.o: $(src)ray.cc triangle.o
#	$(CC) -c $(src)ray.cc -o $(build)ray.o

run:
	./bin/GI-Ray

clear:
	rm -rf $(build)*.o $(bin)$(execfile)
