#pragma once

#include <cmath>
#include <iostream>
#ifdef USING_SSE
#include <xmmintrin.h>
#include <emmintrin.h>
#ifdef __SSE4_1__
#include <smmintrin.h>
#endif
#endif

class vec3 {
  public:
    union {
      struct { float x, y, z; };
      struct { float r, g, b; };
      float v[3];
#ifdef USING_SSE
      __m128 xyzw;
#endif
    };

  public:
    inline vec3(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) { }
    inline vec3(float vec[3]) : x(vec[0]), y(vec[1]), z(vec[2]) { }
    inline vec3(const vec3& vec) : x(vec.x), y(vec.y), z(vec.z) { }

    inline vec3 operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-x, -y, -z); }

#ifdef USING_SSE
    inline vec3(const __m128& _xyzw) : xyzw(_xyzw) { }
    inline vec3 operator+(const vec3& lhs) const { return vec3(_mm_add_ps(xyzw, lhs.xyzw)); }
    inline vec3 operator-(const vec3& lhs) const { return vec3(_mm_sub_ps(xyzw, lhs.xyzw)); }
    inline vec3 operator*(const float lhs) const { return vec3(_mm_mul_ps(xyzw, _mm_load1_ps(&lhs))); }
    inline vec3 operator/(const float lhs) const { return vec3(_mm_div_ps(xyzw, _mm_load1_ps(&lhs))); }
    inline vec3& operator+=(const vec3& lhs) { xyzw = _mm_add_ps(xyzw, lhs.xyzw); return *this; }
    inline vec3& operator-=(const vec3& lhs) { xyzw = _mm_sub_ps(xyzw, lhs.xyzw); return *this; }
    inline vec3& operator*=(const float lhs) { xyzw = _mm_mul_ps(xyzw, _mm_load1_ps(&lhs)); return *this; }
    inline vec3& operator/=(const float lhs) { xyzw = _mm_sub_ps(xyzw, _mm_load1_ps(&lhs)); return *this; }
    inline float dot(const vec3& lhs) const {
#ifdef __SSE4_1__
      __m128 res = _mm_dp_ps(xyzw, lhs.xyzw, 0x77);
      return ((vec3&)res).x;
#else
      __m128 res = _mm_mul_ps(xyzw, lhs.xyzw);
      return res.x + res.y + res.z;
#endif
    }
    inline vec3 cross(const vec3& lhs) const {
      __m128 a = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 0, 2, 1));
      __m128 b = _mm_shuffle_ps(lhs.xyzw, lhs.xyzw, _MM_SHUFFLE(3, 1, 0, 2));
      a = _mm_mul_ps(a, b);
      __m128 c = _mm_shuffle_ps(lhs.xyzw, lhs.xyzw, _MM_SHUFFLE(3, 0, 2, 1));
      __m128 d = _mm_shuffle_ps(xyzw, xyzw, _MM_SHUFFLE(3, 1, 0, 2));
      c = _mm_mul_ps(c, d);
      return vec3(_mm_sub_ps(a, c));
    }
    inline void normalize() {
#ifdef __SSE4_1__
      xyzw = _mm_div_ps(xyzw, _mm_sqrt_ps(_mm_dp_ps(xyzw, xyzw, 0x77)));
#else
      float n = dot(*this);
      xyzw = _mm_div_ps(xyzw, _mm_load1_ps(&n));
#endif
    }
    inline vec3 mul(const vec3& lhs) const { return vec3(_mm_mul_ps(xyzw, lhs.xyzw)); }
    inline vec3 div(const vec3& lhs) const { return vec3(_mm_div_ps(xyzw, lhs.xyzw)); }
    inline vec3 max(const vec3& lhs) const { return vec3(_mm_max_ps(xyzw, lhs.xyzw)); }
    inline vec3 min(const vec3& lhs) const { return vec3(_mm_min_ps(xyzw, lhs.xyzw)); }

#else
    inline vec3 operator+(const vec3& lhs) const { return vec3(x + lhs.x, y + lhs.y, z + lhs.z); }
    inline vec3 operator+(const float k) const { return vec3(x + k, y + k, z + k); }
    inline vec3 operator-(const float k) const { return vec3(x - k, y - k, z - k); }
    inline vec3 operator-(const vec3& lhs) const { return vec3(x - lhs.x, y - lhs.y, z - lhs.z); }
    inline vec3 operator*(const float lhs) const { return vec3(x * lhs, y * lhs, z * lhs); }
    inline vec3 operator/(const float lhs) const { return vec3(x / lhs, y / lhs, z / lhs); }
    inline vec3& operator+=(const vec3& lhs) { x += lhs.x; y += lhs.y; z += lhs.z; return *this; }
    inline vec3& operator-=(const vec3& lhs) { x -= lhs.x; y -= lhs.y; z -= lhs.z; return *this; }
    inline vec3& operator*=(const float lhs) { x *= lhs; y *= lhs; z *= lhs; return *this; }
    inline vec3& operator/=(const float lhs) { x /= lhs; y /= lhs; z /= lhs; return *this; }
    inline float dot(const vec3& lhs) const { return x*lhs.x + y*lhs.y + z*lhs.z; }
    inline vec3 cross(const vec3& lhs) const { return vec3(y * lhs.z - z * lhs.y, z * lhs.x - x * lhs.z, x * lhs.y - y * lhs.x);}
    inline void normalize() { float n = sqrt(x*x + y*y + z*z); x /= n; y /= n; z /= n; }
    inline vec3 mul(const vec3& lhs) const { return vec3(x*lhs.x, y*lhs.y, z*lhs.z); }
    inline vec3 div(const vec3& lhs) const { return vec3(x/lhs.x, y/lhs.y, z/lhs.z); }
    inline vec3 max(const vec3& lhs) const { return vec3(std::max(x, lhs.x), std::max(y, lhs.y), std::max(z, lhs.z)); }
    inline vec3 min(const vec3& lhs) const { return vec3(std::min(x, lhs.x), std::min(y, lhs.y), std::min(z, lhs.z)); }
#endif
    inline float norm2() const { return dot(*this); }
    inline float norm() const { return sqrt(norm2()); }
    inline vec3 unit() const { vec3 res(*this); res.normalize(); return res; }
};

#ifdef USING_SSE
inline vec3 operator*(const float rhs, const vec3& lhs) { return vec3(_mm_mul_ps(lhs.xyzw, _mm_load1_ps(&rhs))); }
#else
inline vec3 operator*(const float rhs, const vec3& lhs) { return lhs * rhs; }
#endif

std::ostream& operator<<(std::ostream& os, const vec3& vec) {
  os << '(' << vec.x << ',' << vec.y << ',' << vec.z << ')'; 
  return os;
}

float length(vec3 p){
  return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

vec3 abs(vec3 p){
  p.x = abs(p.x);
  p.y = abs(p.y);
  p.z = abs(p.z);
  return p;
}

// vec3 cmp(vec3 p, float k){
//   p.x = max(p.x, k);
//   p.y = max(p.y, k);
//   p.z = max(p.z, k);
//   return p;
// }
