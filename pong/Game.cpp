//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

// TODO

Game::Game() {
	// instantiate wall dimensions
	top_wall.x = 0; top_wall.y = 0; top_wall.w = WINDOW_W; top_wall.h = WALL_THICC;
	bot_wall.x = 0; bot_wall.y = WINDOW_H - WALL_THICC; bot_wall.w = WINDOW_W; bot_wall.h = WALL_THICC;
	rht_wall.x = WINDOW_W - WALL_THICC; rht_wall.y = WALL_THICC; rht_wall.w = WALL_THICC; rht_wall.h = WINDOW_H - (2 * WALL_THICC);

	// instantiate paddle dimensions
	paddle_cent.x = (WINDOW_W / 15); paddle_cent.y = (WINDOW_H / 2);
	paddle_rect.x = paddle_cent.x - (WALL_THICC / 2);
	paddle_rect.y = paddle_cent.y - (PADL_HIGHT / 2);
	paddle_rect.h = PADL_HIGHT;
	paddle_rect.w = WALL_THICC;

	// instantiate ball dimensions
	ball_cent.x = WINDOW_W / 2; ball_cent.y = WINDOW_H / 2;
	ball_rect.x = ball_cent.x - (WALL_THICC / 2);
	ball_rect.y = ball_cent.y - (WALL_THICC / 2);
	ball_rect.h = WALL_THICC;
	ball_rect.w = WALL_THICC;
}

bool Game::initialize() {

	// intialize sdl and error check
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// create sdl window and store its pointer
	my_window = SDL_CreateWindow("It's Pong, BABY!", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0/*SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_ALLOW_HIGHDPI*/);

	// error check
	if (!my_window) {
		SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	// create sdl renderer and store its pointer
	my_renderer = SDL_CreateRenderer(my_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// error check
	if (!my_renderer) {
		SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	// initialize SDL_GetTicks()
	prev_time = SDL_GetTicks();

	return true;
}

void Game::runLoop() {
	// run game loop
	while (!off) {
		processInput();
		updateGame();
		generateOutput();
	}
}

void Game::shutdown() {
	// destroy renderer
	SDL_DestroyRenderer(my_renderer);

	// destroy window
	SDL_DestroyWindow(my_window);

	// quit SDL
	SDL_Quit();
}

void Game::processInput() {
	// var to hold events
	SDL_Event event;
	// go through all events in queue
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		// quit game
		case SDL_QUIT : 
			off = true;
			break;
		default:
			break;
		}
	}
	
	// array of states, each cell being the state of a key
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// if escape key pressed, then quit game
	if (state[SDL_SCANCODE_ESCAPE]) {
		off = true;
		return;
	}
	
	// get player's directional input for paddle
	paddle_dir = state[SDL_SCANCODE_DOWN] - state[SDL_SCANCODE_UP];
}

void Game::updateGame() {
	// Frame limit to 60 fps
	while (curr_time - prev_time < DELTA_MIN)
		curr_time = SDL_GetTicks();

	// calculate and store delta time; stored as a float in seconds
	delt_time = (curr_time - prev_time) / 1000.f;
	prev_time = curr_time;

	// cap maximum delta time; so that things don't move too quick
	if (delt_time > DELTA_CAP)
		delt_time = DELTA_CAP;

	// Update all game objects by delta time

	// move paddle based off of player input
	paddle_cent.y += static_cast<int>(paddle_dir * delt_time * PADLE_SPD);
	// collision handling for paddle on walls
	if (paddle_cent.y - (PADL_HIGHT / 2) < top_wall.y + top_wall.h)
		paddle_cent.y = top_wall.y + top_wall.h + (PADL_HIGHT / 2);
	if (paddle_cent.y + (PADL_HIGHT / 2) > bot_wall.y)
		paddle_cent.y = bot_wall.y - (PADL_HIGHT / 2);
	// update the paddle's rectangle
	paddle_rect.y = paddle_cent.y - (PADL_HIGHT / 2);

	// update ball position and velocities
	ball_cent.x += static_cast<int>(ball_dir.x * delt_time * BALL_SPD * spd_mod);
	ball_cent.y += static_cast<int>(ball_dir.y * delt_time * BALL_SPD * spd_mod);

	// if ball leaves screen, game over
	if (ball_cent.x + (WALL_THICC / 2) < 0) {
		off = true;
		return;
	}

	// collision handling for ball on walls
	if (ball_cent.y - (WALL_THICC / 2) < top_wall.y + top_wall.h) {
		ball_cent.y = top_wall.y + top_wall.h + (WALL_THICC / 2);
		ball_dir.y *= -1;
	}
	if (ball_cent.y + (WALL_THICC / 2) > bot_wall.y) {
		ball_cent.y = bot_wall.y - (WALL_THICC / 2);
		ball_dir.y *= -1;
	}
	if (ball_cent.x + (WALL_THICC / 2) > rht_wall.x) {
		ball_cent.x = rht_wall.x - (WALL_THICC / 2);
		ball_dir.x *= -1;
	}
	// collision handling when ball hits paddle
	if ( (ball_cent.x - (WALL_THICC / 2) < paddle_rect.x + paddle_rect.w) 
			&& (ball_cent.x + (WALL_THICC / 2) > paddle_rect.x)
			&& (ball_cent.y + (WALL_THICC / 2) > paddle_cent.y - (PADL_HIGHT / 2))
			&& (ball_cent.y - (WALL_THICC / 2) < paddle_cent.y + (PADL_HIGHT / 2)) ) {
		ball_cent.x = paddle_cent.x + WALL_THICC;
		ball_dir.x *= -1;
		spd_mod += 0.15;
	}


	// update ball's rectangle
	ball_rect.x = ball_cent.x - (WALL_THICC / 2);
	ball_rect.y = ball_cent.y - (WALL_THICC / 2);
}

void Game::generateOutput() {
	// clear back buffer; w/ error check
	if (SDL_SetRenderDrawColor(my_renderer, 0, 0, 255, 255) < 0) {
		SDL_Log("Unable to set renderer color: %s", SDL_GetError());
		off = true;
		return;
	}
	if (SDL_RenderClear(my_renderer) < 0) {
		SDL_Log("Unable to clear renderer: %s", SDL_GetError());
		off = true;
		return;
	}

	// draw game objects
	// walls
	if (SDL_SetRenderDrawColor(my_renderer, 255, 255, 255, 255) < 0) {
		SDL_Log("Unable to set renderer color: %s", SDL_GetError());
		off = true;
		return;
	}
	SDL_RenderFillRect(my_renderer, &top_wall);
	SDL_RenderFillRect(my_renderer, &bot_wall);
	SDL_RenderFillRect(my_renderer, &rht_wall);

	// paddle
	if (SDL_SetRenderDrawColor(my_renderer, 255, 0, 0, 255) < 0) {
		SDL_Log("Unable to set renderer color: %s", SDL_GetError());
		off = true;
		return;
	}
	SDL_RenderFillRect(my_renderer, &paddle_rect);

	// ball
	if (SDL_SetRenderDrawColor(my_renderer, 0, 255, 0, 255) < 0) {
		SDL_Log("Unable to set renderer color: %s", SDL_GetError());
		off = true;
		return;
	}
	SDL_RenderFillRect(my_renderer, &ball_rect);

	// present
	SDL_RenderPresent(my_renderer);
}