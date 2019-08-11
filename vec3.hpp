#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>

struct vec3 {
	explicit vec3() = default;
	explicit vec3(float vX) : vector { vX, vX, vX } {}
	explicit vec3(float v0, float v1, float v2) : vector{ v0, v1, v2 } {}
	vec3& operator=(const vec3& rhs) = default;

	inline float  operator[](int i) const { return vector[i]; }
	inline float& operator[](int i)       { return vector[i]; }

	inline const vec3 operator+() const { return *this; }
	inline const vec3 operator-() const { return vec3(-vector[0], -vector[1], -vector[2]); }

	inline vec3 operator+=(const vec3& rhs);
	inline vec3 operator-=(const vec3& rhs);
	inline vec3 operator*=(const vec3& rhs);
	inline vec3 operator/=(const vec3& rhs);
	inline vec3 operator*=(const float rhs);
	inline vec3 operator/=(const float rhs);

	inline float SqrLength() const;
	inline float Length() const;
	inline vec3& Normalize();

	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float u, v, w; };
		float vector[3];
	};
};

//////////////////////////////////////////////////////////////////////////
// vec3 binary operators
inline vec3 operator+(const vec3& lhs, const vec3& rhs) {
	return vec3(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]);
}

inline vec3 operator-(const vec3& lhs, const vec3& rhs) {
	return vec3(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
}

inline vec3 operator*(const vec3& lhs, const vec3& rhs) {
	return vec3(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}

inline vec3 operator/(const vec3& lhs, const vec3& rhs) {
	return vec3(lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2]);
}

inline vec3 operator*(float lhs, const vec3& rhs) {
	return vec3(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]);
}

inline vec3 operator*(const vec3& lhs, float rhs) {
	return vec3(rhs * lhs[0], rhs * lhs[1], rhs * lhs[2]);
}

inline vec3 operator/(const vec3& lhs, float rhs) {
	return vec3(lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs);
}

//////////////////////////////////////////////////////////////////////////
// compound assignment operators
inline vec3 vec3::operator+=(const vec3& rhs) { *this = *this + rhs; return *this; }
inline vec3 vec3::operator-=(const vec3& rhs) { *this = *this - rhs; return *this; }
inline vec3 vec3::operator*=(const vec3& rhs) { *this = *this * rhs; return *this; }
inline vec3 vec3::operator/=(const vec3& rhs) { *this = *this / rhs; return *this; }
inline vec3 vec3::operator*=(const float rhs) { *this = *this * rhs; return *this; }
inline vec3 vec3::operator/=(const float rhs) { *this = *this / rhs; return *this; }

//////////////////////////////////////////////////////////////////////////
// products & operations
inline float Dot(const vec3& lhs, const vec3& rhs) {
	return (lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]);
}

inline vec3 Cross(const vec3& lhs, const vec3& rhs) {
	float a = lhs.y * rhs.z - lhs.z * rhs.y;
	float b = lhs.z * rhs.x - lhs.x * rhs.z;
	float c = lhs.x * rhs.y - lhs.y * rhs.x;
	return vec3(a, b, c);
}

inline float vec3::SqrLength() const {
	return Dot(*this, *this);
}

inline float vec3::Length() const {
	return sqrtf(Dot(*this, *this));
}

inline vec3 Normalized(const vec3& vector) {
	return vector / vector.Length();
}

inline vec3& vec3::Normalize() {
	return *this = Normalized(*this);
}

inline vec3 Lerp(const vec3& a, const vec3& b, const float t) {
	return (1 - t) * a + t * b;
}

inline vec3 Reflect(const vec3& vector, const vec3& n) {
	return vector - 2 * Dot(vector, n) * n;
}

inline bool Refract(const vec3& vector, const vec3& n, float nint, vec3& outRefracted) {
	vec3 vnorm = Normalized(vector);
	float dt = Dot(vnorm, n);
	float discr = 1.0f - nint * nint * (1 - dt * dt);

	if (discr > 0) {
		outRefracted = nint * (vnorm - n * dt) - n * sqrtf(discr);
		return true;
	}
  return false;
}

//////////////////////////////////////////////////////////////////////////
// io
inline std::ostream& operator<<(std::ostream& os, const vec3& vector) {
	os << vector.x << " " << vector.y << " " << vector.z;
	return os;
}

//////////////////////////////////////////////////////////////////////////
// other functions

float RandFloat01() {
    return (rand() % (RAND_MAX))/ float(RAND_MAX); // REPLACE THIS
}

vec3 RandomInUnitSphere() {
	vec3 p;
	do {
		// p = vec3(drand48(), drand48(), drand48());
		p = vec3(RandFloat01(), RandFloat01(), RandFloat01());
	} while(p.SqrLength() >= 1);

	return p;
}

vec3 RandomInUnitDisk() {
	vec3 p;
	do {
		p = 2.f * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
	} while (p.SqrLength() >= 1);
	return p;
}

vec3 LinearToSRGB(vec3 rgb) {
	return vec3(std::max(1.055f * powf(rgb.r, 0.416666667f) - 0.055f, 0.0f),
				std::max(1.055f * powf(rgb.g, 0.416666667f) - 0.055f, 0.0f),
				std::max(1.055f * powf(rgb.b, 0.416666667f) - 0.055f, 0.0f));
}
