#include "intersection_point.h"

IntersectionPoint::IntersectionPoint(Vertex position,
                                     Direction normal,
                                     Material material)
    : position_(position), normal_(normal), material_(material) {}
