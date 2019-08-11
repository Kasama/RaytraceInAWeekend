#pragma once
#include "vec3.hpp"

class ray {
public:
  vec3 originVec, directionVec;
  ray() {}
  ray(const vec3& origin, const vec3& direction) {
    originVec = origin;
    directionVec = direction;
  }
  vec3 origin()          const { return originVec; }
  vec3 direction()       const { return directionVec; }
  vec3 point_at(float t) const { return originVec + t * directionVec; }
};
