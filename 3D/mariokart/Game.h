#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Player;
class HeightMap;

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	HeightMap* getHeightMap() const { return my_height_map; }
	class Renderer* GetRenderer() {	return mRenderer; }
	const Player* getPlayer() const { return my_player; }
	const bool& isRunning() const { return mIsRunning; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// sound map
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// height map
	HeightMap* my_height_map = nullptr;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	// game's player
	Player* my_player = nullptr;

	// int to keep track of what 1000 the player passed last; used for obstacle spawning
	int thou_count = -1;
	// index of what obstacle file to use; after 20, a random number is used
	int obstacle_index = 0;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
};
