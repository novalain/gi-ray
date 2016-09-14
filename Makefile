CC=g++ -std=c++14
src=./src/
include=./include/
geometry=./src/geometry/
bin=./bin/
build=./build/
execfile=GI-Ray

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: raytracer #allinone

allinone:
	$(CC) $(src)main.cc $(src)scene.cc $(src)camera.cc $(geometry)vertex.cc $(geometry)triangle.cc $(src)ray.cc -o $(bin)$(execfile) -I$(include) #-Wall

raytracer: $(build)main.o
	$(CC) $(build)main.o $(build)scene.o $(build)camera.o $(build)vertex.o $(build)triangle.o $(build)ray.o $(build)pixel.o -o $(bin)$(execfile) #-v -Wall

 $(build)main.o: $(src)main.cc $(build)camera.o $(build)ray.o $(build)scene.o
	$(CC) -c $(src)main.cc -o $(build)main.o -I$(include)

 $(build)camera.o: $(src)camera.cc $(build)pixel.o
	$(CC) -c $(src)camera.cc -o $(build)camera.o -I$(include)

 $(build)vertex.o: $(geometry)vertex.cc
	$(CC) -c $(geometry)vertex.cc -o $(build)vertex.o -I$(include)

 $(build)triangle.o: $(geometry)triangle.cc  $(build)vertex.o
	$(CC) -c $(geometry)triangle.cc -o $(build)triangle.o -I$(include)

 $(build)ray.o: $(src)ray.cc  $(build)triangle.o
	$(CC) -c $(src)ray.cc -o $(build)ray.o -I$(include)

 $(build)pixel.o: $(src)pixel.cc $(build)ray.o
	$(CC) -c $(src)pixel.cc -o $(build)pixel.o -I$(include)

 $(build)scene.o: $(src)scene.cc $(build)triangle.o
	$(CC) -c $(src)scene.cc -o $(build)scene.o -I$(include)

run:
	./bin/GI-Ray

clear:
	rm -rf $(build)*.o $(bin)$(execfile)
