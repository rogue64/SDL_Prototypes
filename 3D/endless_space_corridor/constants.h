#pragma once

// game global constants
const int SCREEN_W = 1024;
const int SCREEN_H = 768;
const int NEAR_PLANE_D = 10;
const int FAR_PLANE_D = 10000;
const float FIELD_O_VIEW = 1.22f; // in radians
const int TARG_D = 20;
const int VERT_D = 100;
const int HORZ_D = 300;
const int SEC = 1000;

// player global constants
namespace plyr_c {
	const int WIDTH = 100;
	const int HEIGHT = 40;
	const int DEPTH = 60;
	const float FORWRD_SPD = 400.f;
	const float STEER_SPD = 300.f;
	const float HORZ_LIM = 195.f;
	const float VERT_LIM = 225.f;
};