CC=g++ -std=c++14
src=./src/
include=-I./include/
geo=./src/geometry/
bin=./bin/
bld=./build/
execfile=$(bin)GI-Ray

#CFLAGS= -c -Wall
#WARNINGS = -Wall

all: raytracer #allinone

allinone:
	$(CC) $(src)main.cc $(geo)triangle_custom_shape.cc $(geo)tetrahedron.cc $(src)scene.cc $(src)camera.cc $(geo)triangle.cc $(src)ray.cc -o $(execfile) $(include) #-Wall

raytracer: $(bld)main.o
	$(CC) $(bld)tetrahedron.o $(bld)triangle_custom_shape.o $(bld)main.o $(bld)scene.o $(bld)camera.o $(bld)triangle.o $(bld)ray.o $(bld)pixel.o -o $(execfile) #-v -Wall

$(bld)main.o: $(src)main.cc $(bld)camera.o $(bld)ray.o $(bld)scene.o $(bld)tetrahedron.o $(bld)triangle_custom_shape.o
	$(CC) $(include) -o $(bld)main.o -c $(src)main.cc

$(bld)camera.o: $(src)camera.cc $(bld)pixel.o
	$(CC) $(include) -o $(bld)camera.o -c $(src)camera.cc

$(bld)triangle.o: $(geo)triangle.cc
	$(CC) $(include) -o $(bld)triangle.o -c $(geo)triangle.cc

$(bld)ray.o: $(src)ray.cc
	$(CC) $(include) -o $(bld)ray.o -c $(src)ray.cc

$(bld)pixel.o: $(src)pixel.cc
	$(CC) $(include) -o $(bld)pixel.o -c $(src)pixel.cc

$(bld)scene.o: $(src)scene.cc $(bld)triangle.o
	$(CC) $(include) -o $(bld)scene.o -c $(src)scene.cc

$(bld)tetrahedron.o: $(geo)tetrahedron.cc $(bld)triangle.o
	$(CC) $(include) -o $(bld)tetrahedron.o -c $(geo)tetrahedron.cc

$(bld)triangle_custom_shape.o: $(geo)triangle_custom_shape.cc $(bld)triangle.o
	$(CC) $(include) -o $(bld)triangle_custom_shape.o -c $(geo)triangle_custom_shape.cc

run:
	$(execfile)

clear:
	rm -rf $(bld)*.o $(execfile)

clearbld:
	rm -rf $(bld)*.o
