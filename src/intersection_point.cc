#include "intersection_point.h"

IntersectionPoint::IntersectionPoint(Vertex position,
                                     Direction normal,
                                     Material material,
                                     float z)
    : position_(position), normal_(normal), material_(material), z_(z) {}
