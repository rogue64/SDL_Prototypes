#pragma once
#include "SDL\SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Random.h"
#include "Asteroid.h"
#include <algorithm>
#include <SDL\SDL_image.h>
#include <vector>
#include <unordered_map>

// forward declarations of classes
class Actor;
class Ship;
class Asteroid;

//constants
static const int WINDOW_W = 1024;
static const int WINDOW_H = 768;
static const int SEC = 1000; // a second in ms
static const int FPS = 60;
static const int DELTA_MIN = SEC / FPS;
static const float DELTA_CAP = 0.033; // 


class Game {
private:
	// window for game
	SDL_Window* my_window = nullptr;

	// renderer for window of game
	SDL_Renderer* my_renderer = nullptr;

	// variables for frame limiting
	size_t prev_time = 0, curr_time = 0;
	float delt_time = 0;
	
	// bool to keep track if game is off or not
	bool off = false;

	// vector of Actor pointers
	std::vector<Actor*> my_actors;
	std::vector<Actor*> new_actors;

	// vector to keep track of asteroids in game; for laser collisions
	std::vector<Asteroid*> my_asteroids;

	// vector of SpriteComponent pointers
	std::vector<SpriteComponent*> my_sprites;

	// hash map of textures loaded for the game
	std::unordered_map<std::string, SDL_Texture*> textureMap;

	// reads input, called in game loop
	void processInput();
	
	// updates game-state, called in game loop
	void updateGame();
	
	// shows effects of current game-state, called in game loop
	void generateOutput();

	// load and unload actors to my_vector
	void LoadData();
	void UnloadData();


public: 
	// constructor
	Game();

	// initializer
	bool initialize();

	// shuts down game
	void shutdown();

	// runs game loop
	void runLoop();

	// adds given actor game
	void addActor(Actor* actor);

	// removes Actor matching given pointer
	void removeActor(Actor* actor);

	// adds Asteroid to game
	void addAsteroid(Asteroid* ast);

	// removes Asteroid from game
	void removeAsteroid(Asteroid* ast);

	// adds given spriteComponent to game
	void addSprite(SpriteComponent* sprite);

	// removes sprite given matching pointer
	void removeSprite(SpriteComponent* sprite);

	// returns texture from given file
	SDL_Texture* getTexture(std::string filename);

	// returns pointer to asteroid that givenactor is colliding with; 
	//	returns nullptr when no collision
	Asteroid* asteroidCollision(Actor* actor) const;
	
};