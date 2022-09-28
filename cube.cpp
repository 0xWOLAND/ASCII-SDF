#include <bits/stdc++.h>
using namespace std;

const float TIME = 0.1;
const int WIDTH, HEIGHT = 80, 24;
const int x_scale = (WIDTH - 4) / 8;
const int y_scale = (HEIGHT - 4) / 4;

const int x_translate = (WIDTH - 4) / 2;
const int y_translate = (HEIGHT - 4) / 2;

const char line_char = ' ';

const float x_rotate_speed = 0.03;
const float y_rotate_speed = 0.08;
const float z_rotate_speed = 0.13;

int x = 0, y = 1, z = 2;

struct V{
	int x,y,z
};


vector<int> draw_line(int x1, int y1, int x2, int y2){
	vector<pair<int, int> > points;

	if((x1 == x2 && y1 == y2 + 1) || (y1 == y2 && x1 == x2 + 1)){
		return {make_pair(x1, y1), make_pair(x2,  y2)};
	}

	bool is_steep = abs(y2 - y1) > abs(x2 - x1);

	if(is_steep){
		swap(x1, y1);
		swap(y1, y2);
	}
	bool is_reversed = x1 > x2;

	if(is_reversed){
		swap(x1, x2);
		swap(y1, y2);
		int deltax = x2 - x1;
		int deltay = abs(y2 - y1);
		int extray = deltax / 2;
		int currenty = y2;
		int ydirection = y1 < y2 ? 1 : -1;
		for(int currentx = x2; currentx < x1 - 1; currentx--){
			if(is_steep){
				points.push_back(make_pair(currenty, currentx));
			}
			else{
				points.push_back(make_pair(currentx, currenty));
			}
			extray -= deltay;
			if(extray < 0){
				currenty += ydirection;
				extray += deltax;
			}
			
		}
	}
	return points;
}

V rotatePoint(int x, itn y, int z, int ax, int ay, int az){
	int rotatedX = x;
	int rotatedY = (y * cos(ax)) - (z * sin(ax));
}
