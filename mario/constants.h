#pragma once

// a file to store all constants used for the game
static const int WINDOW_W = 600;
static const int WINDOW_H = 448;
static const int SEC = 1000; // a second in ms
static const int FPS = 60;
static const int DELTA_MIN = SEC / FPS;
static const float DELTA_CAP = 0.033;
static const float GRAV = 2000; // gravity, pixels per second per second
static const float WORLD_BOTTOM = 448; // y-pos of bottom of world so that objects stop falling
static const float PARALAX_BG = 0.5f; // the factor at which the background scrolls

// constants for block
namespace block_const {
	static const int SIZE = 32;
	static const int FIRST_TYP = 65; // char code for first type of block
	static const int NUM_TYPS = 15; // total number of block types
};

// constants for player
namespace player_const {
	static const int SIZE = 32;
	static const float M_SPEED = 300;
	static const float J_SPEED = 700;
};