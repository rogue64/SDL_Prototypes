#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Player;

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void addBlock(class Block* block);
	void removeBlock(class Block* block);
	const std::vector<class Block*>& getBlocks() const { return my_blocks; }

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }
	const Player* getPlayer() const { return my_player; }
	const bool& isRunning() const { return mIsRunning; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// creates blocks at the x coordinate using file with given index
	void createBlocks(int file_i, float x);
	// spawns a set of blocks depending on player position
	void spawnBlocks();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	// All blocks in game at a time
	std::vector<class Block*> my_blocks;

	// game's player
	Player* my_player;

	// int to keep track of what 1000 the player passed last; used for obstacle spawning
	int thou_count = -1;
	// index of what obstacle file to use; after 20, a random number is used
	int obstacle_index = 0;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
};
