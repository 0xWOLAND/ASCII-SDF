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

class vec2 {
  public:
    union {
      struct { float x, y; };
      struct { float r, g; };
      float v[2];
#ifdef USING_SSE
      __m128 xyzw;
#endif
    };

  public:
    inline vec2(float _x = 0.f, float _y = 0.f) : x(_x), y(_y){ }
    inline vec2(float vec[2]) : x(vec[0]), y(vec[1]){ }
    inline vec2(const vec2& vec) : x(vec.x), y(vec.y){ }

    inline vec2 operator+() const { return *this; }
    inline vec2 operator-() const { return vec2(-x, -y); }

    inline vec2 operator+(const vec2& lhs) const { return vec2(x + lhs.x, y + lhs.y); }
    inline vec2 operator+(const float k) const { return vec2(x + k, y + k); }
    inline vec2 operator-(const vec2& lhs) const { return vec2(x - lhs.x, y - lhs.y); }
    inline vec2 operator-(const float k) const { return vec2(x - k, y - k); }
    inline vec2 operator*(const float lhs) const { return vec2(x * lhs, y * lhs); }
    inline vec2 operator/(const float lhs) const { return vec2(x / lhs, y / lhs); }
    inline vec2& operator+=(const vec2& lhs) { x += lhs.x; y += lhs.y; return *this; }
    inline vec2& operator-=(const vec2& lhs) { x -= lhs.x; y -= lhs.y; return *this; }
    inline vec2& operator*=(const float lhs) { x *= lhs; y *= lhs; return *this; }
    inline vec2& operator/=(const float lhs) { x /= lhs; y /= lhs; return *this; }
    inline float dot(const vec2& lhs) const { return x*lhs.x + y*lhs.y; }
    inline vec2 cross(const vec2& lhs) const { return vec2(x * lhs.y - lhs.x * y);}
    inline void normalize() { float n = sqrt(x*x + y*y); x /= n; y /= n; }
    inline vec2 mul(const vec2& lhs) const { return vec2(x*lhs.x, y*lhs.y); }
    inline vec2 div(const vec2& lhs) const { return vec2(x/lhs.x, y/lhs.y); }
    inline vec2 max(const vec2& lhs) const { return vec2(std::max(x, lhs.x), std::max(y, lhs.y)); }
    inline vec2 min(const vec2& lhs) const { return vec2(std::min(x, lhs.x), std::min(y, lhs.y)); }
    inline float norm2() const { return dot(*this); }
    inline float norm() const { return sqrt(norm2()); }
    inline vec2 unit() const { vec2 res(*this); res.normalize(); return res; }
};

#ifdef USING_SSE
inline vec2 operator*(const float rhs, const vec2& lhs) { return vec2(_mm_mul_ps(lhs.xyzw, _mm_load1_ps(&rhs))); }
#else
inline vec2 operator*(const float rhs, const vec2& lhs) { return lhs * rhs; }
#endif

std::ostream& operator<<(std::ostream& os, const vec2& vec) {
  os << '(' << vec.x << ',' << vec.y << ')'; 
  return os;
}

float length(vec2 p){
  return sqrt(p.x*p.x + p.y*p.y);
}

vec2 abs(vec2 p){
  p.x = abs(p.x);
  p.y = abs(p.y);
  return p;
}

// vec2 cmp(vec2 p, float k){
//   p.x = max(p.x, k);
//   p.y = max(p.y, k);
//   return p;
// }