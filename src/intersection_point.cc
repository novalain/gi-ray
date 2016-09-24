#include "intersection_point.h"

// TODO: This doesn't make sense to have a default constructor,
// but required for ray to have this as a member, could use a pointer in ray class instead
IntersectionPoint::IntersectionPoint()
    : material_(Material(0.f, 0.f, 0.f, COLOR_GREEN)) {
  position_ = Vertex(0, 0, 0);
  normal_ = Vertex(0, 0, 0);
}

IntersectionPoint::IntersectionPoint(Vertex position,
                                     Direction normal,
                                     Material material)
    : position_(position), normal_(normal), material_(material) {}
