CC=g++ -std=c++14
src=./src/
include=./include/
geometry=geometry/
bin=./bin/
build=./build/
execfile=GI-Ray

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: allinone #raytracer

allinone:
	$(CC) $(src)main.cc $(src)scene.cc $(src)camera.cc $(src)$(geometry)vertex.cc $(src)$(geometry)triangle.cc $(src)ray.cc -o $(bin)$(execfile) -I$(include) -Wall

raytracer: $(build)main.o
	$(CC) $(build)main.o $(build)camera.o $(build)vertex.o $(build)triangle.o $(build)ray.o $(build)pixel.o -o $(bin)$(execfile) #-Wall

 $(build)main.o: $(src)main.cc $(build)camera.o $(build)ray.o
	$(CC) -c $(src)main.cc -o $(build)main.o

 $(build)camera.o: $(src)camera.cc $(build)pixel.o
	$(CC) -c $(src)camera.cc -o $(build)camera.o

 $(build)vertex.o: $(src)vertex.cc
	$(CC) -c $(src)vertex.cc -o $(build)vertex.o

 $(build)triangle.o: $(src)triangle.cc  $(build)vertex.o
	$(CC) -c $(src)triangle.cc -o $(build)triangle.o

 $(build)ray.o: $(src)ray.cc  $(build)triangle.o
	$(CC) -c $(src)ray.cc -o $(build)ray.o

 $(build)pixel.o: $(src)pixel.cc $(build)ray.o
	$(CC) -c $(src)pixel.cc -o $(build)pixel.o

run:
	./bin/GI-Ray

clear:
	rm -rf $(build)*.o $(bin)$(execfile)
