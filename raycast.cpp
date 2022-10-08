#include <bits/stdc++.h>
#include <unistd.h>
#include "vec3.h"
#include "vec2.h"
using namespace std;
#define WIDTH 75
#define HEIGHT 150
#define EPSILON 0.2

int main() {

	float time = 0, time_step = 1 / (float) 12;
	vec2 iResolution(WIDTH, HEIGHT);
	while(1){
		printf("\x1b[H");
		for(float r = WIDTH - 1; r >= 0; r--){
			for(float c = 0; c < HEIGHT; c++){
				vec2 fragCoord(c, r * 2.0);
				// fragCoord *= EPSILON;
				// Normalized pixel coordinates (from 0.0 to 1.0)
				vec2 uv = fragCoord / length(iResolution);
				// Draw a circle whose center depends on what time it is
				// vec2 shifted = uv - vec2(0.5, (1.0 + cos(time)) / 2.0);
				vec2 shifted = uv - vec2((sin(time) + 1.0)/2.0, (1.0 + cos(time)) / 2.0);
				// printf("(%d, %d)\n", r, c);
				// cout << "UV: " << uv << "\n";
				if (shifted.dot(shifted) < 0.03) {
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
		usleep(10000);
		// fflush(stdout);
	}
}