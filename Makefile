CLANG=clang++ -openmp
GNU=g++ -fopenmp
src=./src/
include=-I./include/
geo=./src/geometry/
bin=./bin/
bld=./build/
flags=-std=c++14
#Test server doens't support multithreading
flagstravis=-std=c++14
execfile=$(bin)GI-Ray
allsrcfiles=$(src)main.cc $(src)intersection_point.cc $(src)material.cc $(geo)sphere.cc $(geo)tetrahedron.cc $(src)scene.cc $(src)camera.cc $(src)raytracer.cc $(geo)triangle.cc $(src)ray.cc $(src)point_light.cc $(include)
compalltravis=$(flagstravis) $(allsrcfiles)

#CFLAGS= -c -Wall
#WARNINGS = -Wall

allinone_clang: CC=$(CLANG)
allinone_clang: allinone

allinone_gnu: CC=$(GNU)
allinone_gnu: allinone

gnu: CC=$(GNU)
gnu: raytracer

clang: CC=$(CLANG)
clang: raytracer

travistests:
	$(CXX) $(compalltravis) #-Wall

allinone:
	$(CC) $(flags) $(allsrcfiles) -o $(execfile) #-Wall

raytracer: $(bld)main.o
	$(CC) $(flags) $(bld)intersection_point.o $(bld)material.o $(bld)point_light.o $(bld)sphere.o $(bld)tetrahedron.o $(bld)main.o $(bld)scene.o $(bld)camera.o $(bld)raytracer.o $(bld)triangle.o $(bld)ray.o $(bld)pixel.o -o $(execfile) #-v -Wall

$(bld)main.o: $(src)main.cc $(bld)intersection_point.o $(bld)material.o $(bld)camera.o $(bld)raytracer.o $(bld)sphere.o $(bld)ray.o $(bld)scene.o $(bld)tetrahedron.o $(bld)point_light.o
	$(CC) $(flags) $(include) -o $(bld)main.o -c $(src)main.cc

$(bld)intersection_point.o:	$(src)intersection_point.cc
	$(CC) $(flags) $(include) -o $(bld)intersection_point.o -c $(src)intersection_point.cc

$(bld)material.o:	$(src)material.cc
	$(CC) $(flags) $(include) -o $(bld)material.o -c $(src)material.cc

$(bld)camera.o: $(src)camera.cc $(bld)pixel.o $(bld)raytracer.o
	$(CC) $(flags) $(include) -o $(bld)camera.o -c $(src)camera.cc

$(bld)raytracer.o: $(src)raytracer.cc  $(bld)ray.o
	$(CC) $(flags) $(include) -o $(bld)raytracer.o -c $(src)raytracer.cc

$(bld)triangle.o: $(geo)triangle.cc $(src)material.cc
	$(CC) $(flags) $(include) -o $(bld)triangle.o -c $(geo)triangle.cc

$(bld)ray.o: $(src)ray.cc
	$(CC) $(flags) $(include) -o $(bld)ray.o -c $(src)ray.cc

$(bld)pixel.o: $(src)pixel.cc
	$(CC) $(flags) $(include) -o $(bld)pixel.o -c $(src)pixel.cc

$(bld)scene.o: $(src)scene.cc $(bld)triangle.o $(bld)point_light.o
	$(CC) $(flags) $(include) -o $(bld)scene.o -c $(src)scene.cc

$(bld)tetrahedron.o: $(geo)tetrahedron.cc $(bld)triangle.o
	$(CC) $(flags) $(include) -o $(bld)tetrahedron.o -c $(geo)tetrahedron.cc

$(bld)point_light.o: $(src)point_light.cc
	$(CC) $(flags) $(include) -o $(bld)point_light.o -c $(src)point_light.cc

$(bld)sphere.o: $(geo)sphere.cc
	$(CC) $(flags) $(include) -o $(bld)sphere.o -c $(geo)sphere.cc

run:
	$(execfile)

clean:
	rm -rf $(bld)*.o $(execfile) ./results/*.ppm

clearbld:
	rm -rf $(bld)*.o
