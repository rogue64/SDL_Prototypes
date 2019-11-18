#pragma once
#include "SDL\SDL.h"

//constants
static const int WINDOW_W = 1024;
static const int WINDOW_H = 768;
//static const int WALL_THICC = 15;
static const int WALL_THICC = (WINDOW_W - WINDOW_H) / 15;
//static const int PADL_HIGHT = 64;
static const int PADL_HIGHT = WINDOW_H / 8;
static const int SEC = 1000; // a second in ms
static const int FPS = 60;
static const int DELTA_MIN = SEC / FPS;
static const float DELTA_CAP = 0.033; // 
static const int PADLE_SPD = 500;
static const int BALL_SPD = 250;

class Game {
private:
	// window for game
	SDL_Window* my_window = nullptr;

	// renderer for window of game
	SDL_Renderer* my_renderer = nullptr;

	// rectangles that make up the walls of the game; and for the paddle and ball
	SDL_Rect top_wall, bot_wall, rht_wall, paddle_rect, ball_rect;

	// sdl points that store the center of the paddle and ball, respectively
	SDL_Point paddle_cent, ball_cent;

	// variables for frame limiting
	size_t prev_time = 0, curr_time = 0;
	float delt_time = 0;

	// speed modifier for when speed increases
	float spd_mod = 1.0;

	// direction of the paddle
	int paddle_dir = 0;

	// sdl point to store directions of ball
	SDL_Point ball_dir = {1, -1};
	
	// bool to keep track if game is off or not
	bool off = false;

	// reads input, called in game loop
	void processInput();
	
	// updates game-state, called in game loop
	void updateGame();
	
	// shows effects of current game-state, called in game loop
	void generateOutput();


public: 
	// constructor
	Game();

	// initializer
	bool initialize();

	// shuts down game
	void shutdown();

	// runs game loop
	void runLoop();
};
