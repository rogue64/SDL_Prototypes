#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>
#include "CheckPoint.h"
#include "SecurityCam.h"

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
	void addBlock(class Actor* actor);
	void removeBlock(class Actor* actor);
	const int& getCoins() const { return coin_count; }
	void addCoin();

	const std::vector<class Actor*>& getBlocks() const { return my_blocks; }

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }
	const Player* getPlayer() const { return my_player; }
	void setPlayer(Player* player) { my_player = player; }
	const bool& isRunning() const { return mIsRunning; }
	
	void addCheckpoint(CheckPoint* cp);
	void popCheckpoint();
	CheckPoint* getActiveCheckPoint();

	// sets the next level string
	void setNextLevel(std::string level) { next_level = level; }

	void setCPString(std::string in) { cp_string = in; }
	const std::string& getCPString() { return cp_string; }
	const float getTime() const { return time; }

	void addCam(SecurityCam* in) { my_cams.push_back(in); }
	void removeCam(SecurityCam* in);
	const std::vector<SecurityCam*>& getCams() const { return my_cams; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// sound map
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	// seperate vector of actors for specific actors
	std::vector<class Actor*> my_blocks;
	// queue of checkpoints
	std::queue<CheckPoint*> my_checkpoints;
	// vector of security cameras
	std::vector<SecurityCam*> my_cams;

	// string that determines what next level is when not empty
	std::string next_level = "";
	// sets next level if next_level is set
	void loadNextLevel();

	// game's player
	Player* my_player = nullptr;

	// count of how many coins collected
	int coin_count = 0;
	// float to keep track of time elapsed
	float time = 0;
	// string used to store checkpoint message when changing levels
	std::string cp_string = "";

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;
};
