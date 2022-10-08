#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;


struct vec3 {
	float v[3];

	vec3() {
		memset(v, 0, 3);
		v[0] = v[1] = v[2] = 0.0;
	}

	vec3(float x, float y = 0, float z = 0){
		memset(v, 0.0, 3);
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	float len(){
		float ans = 0;
		for(int i = 0; i < 3; i++){
			ans += (v[i] * v[i]);
		}
		return sqrt(ans);
	}
	void print() {
		for(int i = 0; i < 3; i++){
			printf("%4.3f ", v[i]);
		}
		cout << "\n";
	}

	void normalize() {
		float l = len();
		for(int i = 0; i < 3; i++) v[i] /= l;
	}

	float dot(const vec3& v1){
		float ans = 0;
		for(int i = 0; i < 3; i++){
			ans += v[i] * v1.v[i];
		}
		return ans;
	} 

	vec3 cross(const vec3& other){
		return vec3 
			(
			v[1] * other.v[2] - v[2] * other.v[1],
			v[2] * other.v[0] - v[0] * other.v[2],
			v[0] * other.v[1] - v[1] * other.v[0]
			);
	}

	vec3 operator +(const vec3& v1) const {
		vec3 ans;
		for(int i = 0; i < 3; i++){
			ans.v[i] = v[i] + v1.v[i];
		}
		return ans;
	}


	vec3 operator -(const vec3& v1) const {
		vec3 ans;
		for(int i = 0; i < 3; i++){
			ans.v[i] = v[i] - v1.v[i];
		}
		return ans;
	}


	vec3 operator *(const vec3& v1) const {
		vec3 ans;
		for(int i = 0; i < 3; i++){
			ans.v[i] = v[i] * v1.v[i];
		}
		return ans;
	}

	vec3 operator *(const float k) const {
		vec3 ans;
		for(int i = 0; i < 3; i++){
			ans.v[i] = v[i] * k;
		}
		return ans;
	}

};

struct ray {
	vec3 pos, dir;
	void print() {
		cout << "POS: \n";
		for(int i = 0; i < 3; i++){
			printf("%4.3f ", pos.v[i]);
		}
		cout << "\n";
		cout << "DIR: \n";
		for(int i = 0; i < 3; i++){
			printf("%4.3f ", dir.v[i]);
		}
		cout << "\n";
	}
};

struct intersect {
	vec3 pos, normal;
};

struct sphere {
	vec3 pos;
	float radius;

	bool hits(const ray& r, intersect * ict) {
		vec3 v = pos - r.pos;
		float b = v.dot(r.dir);
		float disc = b * b - v.dot(v) + radius * radius;

		if( disc < 0 ){
			return false;
		}

		if(ict != nullptr){
			float d = sqrt(disc);
			float t1 = b - d;
			float t2 = b + d;
			ict->pos = r.pos + r.dir * (t1 > 0 ? t1 : t2);
			ict->normal = ict->pos - pos;
			ict->normal.normalize();
		}
		return true;
	}
};

#define WIDTH  (79)
#define HEIGHT (37)
const float ASPECT = (WIDTH / (float) (HEIGHT) * (float)(0.5));

#define FOVFactor (float)0.5773502691896257645091487805019574556476017512701268 // 60 degrees FOV(tan((pi/3)/2))
const string shades = ".,-~:;=!*#$@";

vec3 i_(1,0,0);
vec3 j_(0,1,0);
vec3 k_(0,0,1);



int main() {
	string buff;
	vec3 eye(0.0, 2.5, -3.75);
	vec3 look_at(0.0,0.0,0.0);
	vec3 eye_dir = look_at - eye;
	eye_dir.normalize();

	vec3 eye_right = eye_dir.cross(vec3(0, -1, 0));
	vec3 eye_up = eye_right.cross(eye_dir);


	eye_up.normalize();
	eye_right.normalize();
	vec3 light_pos(0, 15, -5);

	sphere ball;

	ball.pos = vec3(0.0, 0.0, 0.0);
	ball.radius = 1;
	intersect ict;
	vec3 light;

	float time = 0, time_step = 1 / (float) 12;
	while(1){
		printf("\x1b[H");
		for(int y = 0; y < HEIGHT; y++){
			for(int x = 0; x < WIDTH; x++){
				ray r;
				r.pos = eye;
				vec3 point = eye_right * FOVFactor * ASPECT * float((x / (float)WIDTH) - (float)0.5) +
					eye_up * FOVFactor * float((y / (float)HEIGHT) - (float)0.5) +
					eye + eye_dir;
				r.dir = point - eye;
				r.dir.normalize();
				if(ball.hits(r, &ict)){
					light = light_pos - ict.pos;
					light.normalize();
					float diff = ict.normal.dot(light);
					if(diff >= 0){
						putchar(shades[(uint32_t)((diff*diff*diff)*(shades.length() - 1))]);
					}
					else{
						putchar('.');
					}
				}
				else{
					putchar(' ');
				}
			}
			putchar('\n');
		}
		time += time_step;
		light_pos = vec3((float)cos(time * (4 * M_PI)) * 15, 10, (float)sin(time * (M_PI * 4)) * 15);
		usleep(100);
	}

}
