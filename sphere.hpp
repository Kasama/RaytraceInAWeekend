#pragma once

#include "vec3.hpp"
#include "ray.hpp"

struct HitRecord {
  float t;
  vec3 intercection;
  vec3 normal;
};

struct Sphere {
  vec3 center;
  float radius;
  bool hit(const ray& r, float t_min, float t_max, HitRecord& rec) const {
    float a = Dot(r.direction(), r.direction());
    float b = 2 * Dot(r.direction(), r.origin() - center);
    float c = Dot(r.origin() - center, r.origin() - center) - (radius * radius);
    float delta = b*b - 4 * a * c; // b² - 4ac

    if (delta < 0) return false;

    auto update_t = [ &rec, t_min, t_max, &r, this ](float t) {
      rec.t = t;
      rec.intercection = r.point_at(t);
      rec.normal = Normalized(rec.intercection - this->center);
    };

    float neg_t = (- b - sqrtf(delta)) / (2 * a);
    if (neg_t < t_max && neg_t > t_min) {
      update_t(neg_t);
      return true;
    }
    float pos_t = (- b + sqrtf(delta)) / (2 * a);
    if (pos_t < t_max && pos_t > t_min) {
      update_t(pos_t);
      return true;
    }

    return false;
  }
};
