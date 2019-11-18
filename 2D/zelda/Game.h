#pragma once
#include "constants.h"
#include "SDL\SDL.h"
#include "SDL\SDL_mixer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "TiledBGComp.h"
#include "Random.h"
#include "Spawner.h"
#include "Player.h"
#include "Door.h"
#include "Collider.h"
#include "Secret.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <SDL\SDL_image.h>
#include <vector>
#include <unordered_map>

class Game {
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

	// adds given collider to game
	void addCollider(Collider* collider);

	// removes given actor to game
	void removeCollider(Collider* collider);

	// returns reference to the game's vector of colliders
	const std::vector<const Collider*>& getColliders() const { return my_colliders; }

	// adds given door to game
	void addDoor(Door* door);

	// removes given door to game
	void removeDoor(Door* door);

	// returns reference to game's vector of doors
	const std::vector<Door*>& getDoors() const { return my_doors; }

	// adds given Secret to game vector
	void addSecret(Secret* secret);

	// removes given secret from game vector
	void removeSecret(Secret* secret);
	
	// returns the Secret vector
	const std::vector<Secret*>& getSecrets() const { return my_secrets; }

	// adds given spriteComponent to game
	void addSprite(SpriteComponent* sprite);

	// removes sprite given matching pointer
	void removeSprite(SpriteComponent* sprite);

	// returns texture from given file
	SDL_Texture* getTexture(std::string filename);

	// returns a mix chunk from given file
	Mix_Chunk* getSound(const std::string& filename);

	// accesses the game's camera variable
	Vector2& getCamera() { return my_camera; }

	// opens doors in a room
	void openDoors(std::string room);

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

	// pointer to game's player
	Player* my_player = nullptr;

	// game's camera position
	Vector2 my_camera = Vector2::Zero;

	// vector of Actor pointers
	std::vector<Actor*> my_actors;
	std::vector<Actor*> new_actors;

	// vector of Collider pointers
	std::vector<const Collider*> my_colliders;

	// vector of Door pointers
	std::vector<Door*> my_doors;

	// vector of Secret pointers
	std::vector<Secret*> my_secrets;

	// vector of SpriteComponent pointers
	std::vector<SpriteComponent*> my_sprites;

	// hash map of textures loaded for the game
	std::unordered_map<std::string, SDL_Texture*> textureMap;

	// hash map of mix chunks loaded for game
	std::unordered_map<std::string, Mix_Chunk*> soundMap;

	// reads input, called in game loop
	void processInput();
	
	// updates game-state, called in game loop
	void updateGame();
	
	// shows effects of current game-state, called in game loop
	void generateOutput();

	// load and unload actors to my_vector
	void LoadData();
	void UnloadData();

	// loads colliders for level using the given CSV file
	void parseCSV(const std::string& filename, const std::string& room);
};