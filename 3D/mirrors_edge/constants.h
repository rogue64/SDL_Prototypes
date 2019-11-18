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
const float MAX_MOUSE_MOV_X = 500.f;
const float MAX_MOUSE_MOV_Y = 500.f;
const Vector3 W_GRAV = Vector3(0, 0, -980.f); // 1 game-unit = 1 cm
const float wall_climb_leeway = 0.25f; // percentage to allow wall-climbing
const float WALL_RUN_SPD_SQ = 350.0f * 350.0f;
const float NORM_F_MAG = 700.0f;
const int MAX_COINS = 55;
const float CP_MESSAGE_LIMIT = 5.f;
const float	RADAR_RANGE = 1500.f;

// player global constants
namespace plyr_c {
	//const float MOV_SPD = 350.f;
	const float MAX_MOV_SPD = 400.f;
	const float MAX_MOV_SPD_SQ = MAX_MOV_SPD * MAX_MOV_SPD;
	const float MOVE_FORCE_MAG = 700.f;
	const Vector3 CLIMB_FORCE = Vector3(0.f, 0.f, 1800.f);
	const Vector3 W_RUN_FORCE = Vector3(0.f, 0.f, 1200.f);
	const float CLIMB_LIMIT = 4.0f * SEC; // seconds we can climb
	//const float JUMP_SPD = 500.f;
	const Vector3 JUMP_FORCE = Vector3(0, 0, 35000.f);
	const float STRAFE_SPD = 350.f;
	const float RAD_PER_SEC_TURNING = 10 * Math::Pi;
	const float ACCL_TIME_LIM = 2.f;
	const float MAX_ACCL = 2500.f;
	const float MIN_ACCL = 1000.f;
	const float FRIC_COEF = 0.9f; // friction coefficient
	const float	COAST_DRAG = 0.975f;
	const float ANGL_ACCL = 30.f * Math::Pi;
	const float ANGL_DRAG = 0.9f;
};