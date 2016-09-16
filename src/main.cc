#include <iostream>
#include <glm/glm.hpp>
#include "commons.h"
#include "camera.h"
#include "triangle.h"
#include "ray.h"
#include "scene.h"
#include "triangle.h"
#include "tetrahedron.h"

int main() {
  std::cout << "Hello World" << std::endl;
  Scene scene;
  Camera cam = Camera(Vertex(-2,0,0), Vertex(-1,0,0), Direction(1,0,0), Direction(0,0,1));
  cam.ClearColorBuffer(glm::vec3(155,45,90));
  // cam.CreateImage("testImg",true);

  Triangle t0 = Triangle(Vertex(2, 3, 4), Vertex(2, 3, 5), Vertex(1, 2, 3));
  Triangle t1 = Triangle(Vertex(2, 3, 4), Vertex(2, 3, 5), Vertex(1, 2, 3));
  Triangle t2 = Triangle(Vertex(2, 3, 4), Vertex(2, 3, 5), Vertex(1, 2, 3));
  Triangle t3 = Triangle(Vertex(2, 3, 4), Vertex(2, 3, 5), Vertex(1, 2, 3));

  Tetrahedron t = Tetrahedron(t0,t1,t2,t3);

  cam.Render(scene);
  cam.CreateImage("max_intensity_ep1",true);
  cam.CreateImage("sqrt_intensity_ep1",false);

  cam.ChangeEyePos();

  cam.Render(scene);
  cam.CreateImage("max_intensity_ep2",true);
  cam.CreateImage("sqrt_intensity_ep2",false);
}
