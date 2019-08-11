#include <iostream>
#include <cstdlib>
#include <vector>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"

#define MAX_COLOR 255.99f

auto normalizeBetween0and1(float y) { return .5f * (y + 1.f); }
vec3 getBackgroundColor(const vec3& direction) {
  vec3 lowColor  { 1.f, 1.f, 1.f };
  vec3 highColor { .5f, .7f, 1.f };

  float directionFrom0to1 = normalizeBetween0and1(direction.y);

  return Lerp(highColor, lowColor, (1.f - directionFrom0to1));
}

vec3 getSceneColor(const ray& r, const std::vector<Sphere>& objects) {

  bool didInterceptAny = false;
  HitRecord closestHitRecord;
  closestHitRecord.t = MAXFLOAT;
  for (const auto& sphere : objects) {
    HitRecord hitRecord;
    bool didIntercept = sphere.hit(r, 0, closestHitRecord.t, hitRecord);
    if (didIntercept) {
      didInterceptAny = true;
      closestHitRecord = hitRecord;
    }
  }

  vec3 sphereColor {
    normalizeBetween0and1(closestHitRecord.normal.z),
    normalizeBetween0and1(closestHitRecord.normal.y),
    normalizeBetween0and1(closestHitRecord.normal.x),
  };

  vec3 unit_direction = Normalized(r.direction());
  return didInterceptAny
    ? sphereColor
    : getBackgroundColor(unit_direction);
}

void printColor(const vec3& col) {
  int ir = int(MAX_COLOR * col.r);
  int ig = int(MAX_COLOR * col.g);
  int ib = int(MAX_COLOR * col.b);

  printf("%d %d %d\n", ir, ig, ib);
}

int main() {
  int factor = 200;
  int nPixelsHorizontal = 2 * factor, nPixelsVertical = 1 * factor;

  printf("P3\n");
  printf("%d %d\n", nPixelsHorizontal, nPixelsVertical);
  printf("255\n");

  vec3 origin          { 0.f, 0.f, 0.f };
  vec3 lowerLeftCorner { -2.f, -1.f, -1.f };
  vec3 horizontal      { 4.f, 0.f, 0.f };
  vec3 vertical        { 0.f, 2.f, 0.f };

  std::vector<Sphere> objects {
    Sphere {
      vec3 {0.f, -100.5f, -1.f},
      100.f,
    },
    Sphere {
      vec3 {0.f, 0.f, -1.f},
      .5f,
    },
  };


  for (int y = nPixelsVertical - 1; y >= 0; y--) {
    for (int x = 0; x < nPixelsHorizontal; x++) {
      vec3 uv { float(x) / float(nPixelsHorizontal), float(y) / float(nPixelsVertical), 0.f };

      vec3 visionDirection = lowerLeftCorner + uv.u * horizontal + uv.v * vertical;
      ray cameraVision(origin, visionDirection);

      vec3 color = getSceneColor(cameraVision, objects);

      printColor(color);
    }
  }
}
