#include <bits/stdc++.h>
#include <unistd.h>
#include "vec3.h"
#include "vec2.h"
using namespace std;
#define WIDTH 50 
#define HEIGHT 100
#define EPSILON 0.2
float Radius = 0.5;
vec2 Center = vec2(0.80, 0.5);

// Sphere signed distance function
float sphereSDF(vec2 pos)
{
    return pow(max(Radius - length(pos - Center), (float)(0.0)), (float)(0.3));
}

int main() {

	float time = 0, time_step = 1 / (float) 12;
	vec2 iResolution(WIDTH, HEIGHT);
	while(1){
		printf("\x1b[H");
		for(float r = WIDTH - 1; r >= 0; r--){
			for(float c = 0; c < HEIGHT; c++){
				vec2 fragCoord(c, r * 2.0);
				vec2 uv = fragCoord / iResolution.x;
                float val = sphereSDF(uv);
				if (val > 0.03) {
					// Varying pixel colour
					// vec3 col = 0.5 + 0.5*cos(time+uv+vec3(0.0,2.0,4.0));
					putchar('#');
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