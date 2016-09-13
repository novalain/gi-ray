#include "scene.h"

Scene::Scene() {
  InitRoom();
  //InitObjects();
}

void Scene::InitRoom() {
  // Floor vertices
  Vertex vfC(  5,  0, -5,  1); // center vertex on floor
  Vertex vf1( -3,  0, -5,  1);
  Vertex vf2(  0, -6, -5,  1);
  Vertex vf3( 10, -6, -5,  1);
  Vertex vf4( 13,  0, -5,  1);
  Vertex vf5( 10,  6, -5,  1);
  Vertex vf6(  0,  6, -5,  1);

  // Ceiling vertices
  Vertex vcC(  5,  0,  5,  1); // center vertex on ceiling
  Vertex vc1( -3,  0,  5,  1);
  Vertex vc2(  0, -6,  5,  1);
  Vertex vc3( 10, -6,  5,  1);
  Vertex vc4( 13,  0,  5,  1);
  Vertex vc5( 10,  6,  5,  1);
  Vertex vc6(  0,  6,  5,  1);

  int count = 0;

  // Floor
  triangles_[count++] = Triangle(vfC, vf6, vf1);
  triangles_[count++] = Triangle(vfC, vf1, vf2);
  triangles_[count++] = Triangle(vfC, vf2, vf3);
  triangles_[count++] = Triangle(vfC, vf3, vf4);
  triangles_[count++] = Triangle(vfC, vf4, vf5);
  triangles_[count++] = Triangle(vfC, vf5, vf6);

  // Ceiling
  triangles_[count++] = Triangle(vcC, vc1, vc6);
  triangles_[count++] = Triangle(vcC, vc2, vc1);
  triangles_[count++] = Triangle(vcC, vc3, vc2);
  triangles_[count++] = Triangle(vcC, vc4, vc3);
  triangles_[count++] = Triangle(vcC, vc5, vc4);
  triangles_[count++] = Triangle(vcC, vc6, vc5);

  /* Counter-clockwise order, starting with front */

  // Wall1
  triangles_[count++] = Triangle(vf2, vc2, vf3);
  triangles_[count++] = Triangle(vf3, vc2, vc3);

  // Wall 2
  triangles_[count++] = Triangle(vf3, vc3, vf4);
  triangles_[count++] = Triangle(vf4, vc3, vc4);

  // Wall 3
  triangles_[count++] = Triangle(vf4, vc4, vf5);
  triangles_[count++] = Triangle(vc4, vc5, vf5);

  // Wall 4
  triangles_[count++] = Triangle(vc5, vf6, vf5);
  triangles_[count++] = Triangle(vc5, vc6, vf6);

  // Wall 5
  triangles_[count++] = Triangle(vc6, vc1, vf6);
  triangles_[count++] = Triangle(vc1, vf1, vf6);

  // Wall 6
  triangles_[count++] = Triangle(vf2, vf1, vc2);
  triangles_[count++] = Triangle(vf1, vc1, vc2);
}