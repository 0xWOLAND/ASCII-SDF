#include <bits/stdc++.h>
#include <unistd.h>
#include "vec3.h"
#include "vec2.h"
using namespace std;
#define WIDTH 100 
#define HEIGHT 200
// ray marching
const int max_iterations = 256;
const float near_distance = 0.001;
const float far_distance = 30.0;

// math
const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;


vec3 fov(float dir, vec2 screen_size, vec2 fragCoord) {
    vec2 xy = fragCoord - screen_size * 0.5;
    float cot_half_fov = tan( (90.0 - dir * 0.5) * DEG_TO_RAD);
    float z = screen_size.y * 0.5 * cot_half_fov;

    vec3 ret(xy.x, xy.y, -z);
    return ret.unit();
}

struct Camera {
    vec3 eye, lookAt, ray;
};

struct mat3 {
    vec3 i, j, k;
    inline mat3(vec3 _i, vec3 _j, vec3 _k){
        i = _i;
        j = _j;
        k = _k;
    }
    inline vec3 operator*(vec3 v) const {
        vec3 ret;
        ret.x = i.x * v.x + j.x * v.y + k.x * v.z;
        ret.y = i.y * v.x + j.y * v.y + k.y * v.z;
        ret.z = i.z * v.x + j.z * v.y + k.z * v.z;
        return ret;
    }
};

struct mat2 {
    vec2 i, j;
    inline mat2(vec2 _i, vec2 _j){
        i = _i;
        j = _j;
    }
    inline vec2 operator*(vec2 v) const {
        vec2 ret;
        ret.x = i.x * v.x + j.x * v.y;
        ret.y = i.y * v.x + j.y * v.y;
        return ret;
    }
};


mat3 lookAtMat(vec3 eye, vec3 lookAt, vec3 up){
	vec3 f = (lookAt - eye).unit();
	vec3 s = f.cross(up).unit();
	vec3 u = s.cross(f);
	return mat3(
		vec3(s),
		vec3(u),
		vec3(-f)
	);
}

Camera setupCamera(vec3 ray, float time){
    Camera camera;
    float posx = sin(time);
    camera.eye = vec3(posx, 5, 7);
    camera.lookAt = vec3(0,0,0);
    camera.ray = lookAtMat(camera.eye, camera.lookAt, vec3(0,1,0)) * ray;
    return camera;
}

float sdfSphere(vec3 p, float r){
    return length(p) - r;
}

vec3 cmp(vec3 v, float k){
    v.x = max(v.x, k);
    v.y = max(v.y, k);
    v.z = max(v.z, k);
    return v;
}

double clamp(double a, double low, double high){
    return a < low ? low : high < a ? high : a;
}

float sdBox( vec3 p, vec3 b ) {
//   vec3 d = abs(p) - b;
//   return min(max(d.x,max(d.y,d.z)),0.0f) + length(d);
    vec3 q = abs(p) - b;
  return length(cmp(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0f);
}

float displacement(vec3 p){
    return sin(20*p.x)*sin(20*p.y)*sin(20*p.z);
}

float sdDeathStar( vec3 p2, float ra, float rb, float d )
{
  // sampling independent computations (only depend on shape)
  float a = (ra*ra - rb*rb + d*d)/(2.0*d);
  float b = sqrt(max(ra*ra-a*a,0.0f));
	
  // sampling dependant computations
  vec2 p = vec2( p2.x, length(vec2(p2.y, p2.z)) );
  if( p.x*b-p.y*a > d*max(b-p.y,0.0f) )
    return length(p-vec2(a,b));
  else
    return max( (length(p          )-ra),
               -(length(p-vec2(d,0))-rb));
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(vec2(p.x, p.z))-t.x,p.y);
  return length(q)-t.y;
}

float opDisplaceTorus(vec3 p, vec2 t)
{
    float d1 = sdTorus(p, t);
    float d2 = displacement(p);
    return d1+d2;
}

float opTwist(vec3 p, vec2 t)
{
    const float k = 1.0; // or some other amount
    float c = cos(k*p.y);
    float s = sin(k*p.y);
    mat2  m = mat2(vec2(c,s),vec2(-s,c));
    vec2 tmp = m * vec2(p.x, p.z);
    vec3  q = vec3(tmp.x, tmp.y,p.y);
    return sdTorus(q, t);
}
float sdOctahedron( vec3 p, float s)
{
  p = abs(p);
  float m = p.x+p.y+p.z-s;
  vec3 q;
       if( 3.0*p.x < m ) q = vec3(p.x, p.y, p.z);
  else if( 3.0*p.y < m ) q = vec3(p.y, p.z, p.x);
  else if( 3.0*p.z < m ) q = vec3(p.z, p.x, p.y);
  else return m*0.57735027;
    
  float k = clamp(0.5*(q.z-q.y+s),0.0,(double)(s)); 
  return length(vec3(q.x,q.y-s+k,q.z-k)); 
}

float distanceField(vec3 p){
    float d = sdfSphere(p + vec3(-4, 3, 0), 0.5);
    float d2 = sdBox( p + vec3(0,2,0), vec3(0.5) );	//object 2
    float d3 = sdTorus( p + vec3(-1,3,8), vec2(1.85, 0.5) );	//object 2
    // float d4 = opDisplaceTorus( p + vec3(-4,2,3), vec2(0.75, 0.55) );	//object 2
    // float d4 = opTwist( p + vec3(0,0,0), vec2(1, 0.82) );	//object 2
    // float d4 = sdDeathStar(p + vec3(-4, 1, 3), 4.2, 3.5, 2.3);
    float d4 = sdOctahedron(p + vec3(-4, 1, 3), 0.25f);
    d = min(d,d2);
    d = min(d,d3);
    d = min(d,d4);
    // d = min(d,d4);


    return d;
}

float rayMarching(vec3 eye, vec3 dir, float depth){
    float t = 0.0f, d = 0.0f;

    for(int i = 0; i < max_iterations; i++){
        vec3 v = eye + dir * t;
        d = distanceField(v);
        // printf("D: %f\n", d);
        if(d < near_distance){
            break;
        }
        t += min(abs(d), 0.1f);
        if(t > depth) {
            break;
        }
    }
    if (d >= near_distance){
        return -1.0f;
    }
    return length(t * dir);
}
float march(Camera cm){
    float depth = far_distance;
    return rayMarching(cm.eye, cm.ray, depth);
}

const string shades = ".,-~:;=!*#$@";


int main() {

    float time = 0, time_step = 1 / (float) 12;
    vec2 iResolution(WIDTH, HEIGHT);
    // iResolution *= 0.5f;
    while(1){
        printf("\x1b[H");
        for(float r = WIDTH - 1; r >= 0; r--){
            for(float c = HEIGHT - 1; c >= 0; c--){
                vec2 fragCoord(c, r * 2);
                vec3 ray = fov(45.0f, iResolution, fragCoord);
                Camera cm = setupCamera(ray, time);
                float val = march(cm);
                if (val > 0.0) {
                    // Varying pixel colour
                    // vec3 col = 0.5 + 0.5*cos(time+uv+vec3(0.0,2.0,4.0));
                    // putchar(shades[(uint32_t)((diff*diff*diff)*(shades.length() - 1))]);
                    putchar('#');
                    // putchar(shades[distShader(val)]);
                } else {
                    // make everything outside the circle black
                    putchar('.');
                }
            }
            putchar('\n');
        }

        time += time_step;
        sleep(1);
        // fflush(stdout);
    }
}
