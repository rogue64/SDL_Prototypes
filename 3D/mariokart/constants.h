#pragma once
#include "Math.h"

// game global constants
const int SCREEN_W = 1024;
const int SCREEN_H = 768;
const int NEAR_PLANE_D = 10;
const int FAR_PLANE_D = 10000;
const float FIELD_O_VIEW = 1.22f; // in radians
const int TARG_D = 50;
const int DEFAULT_HEIGHT = 70;
const int HORZ_D = 120;
const float CAM_SPRING = 256.f;
const float CAM_DAMP = 2.f * Math::Sqrt(CAM_SPRING);
const int SEC = 1000;

// player global constants
namespace plyr_c {
	const float ACCL_TIME_LIM = 2.f;
	const float MAX_ACCL = 2500.f;
	const float MIN_ACCL = 1000.f;
	const float ACCLING_DRAG = 0.9f;
	const float	COAST_DRAG = 0.975f;
	const float ANGL_ACCL = 30.f * Math::Pi;
	const float ANGL_DRAG = 0.9f;
};