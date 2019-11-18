#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "constants.h"
#include "Player.h"
#include "SDL/SDL_mixer.h"
#include "LevelLoader.h"
#include "CheckPointArrow.h"
#include <SDL/SDL_ttf.h>
#include "HUD.h"

class Mesh;

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize() {
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// TODO: Create renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(SCREEN_W, SCREEN_H))
		return false;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// load SDL's fonts system
	TTF_Init();

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
		// check if it is time to load to a new level
		if (next_level != "")
			loadNextLevel();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();

	// update time elapsed
	time += deltaTime;

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy) {
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// TODO: tell renderer to draw
	mRenderer->Draw();
}

void Game::LoadData() {
	// load level
	LevelLoader::Load(this, "Assets/Tutorial.json");

	// create checkpoint arrow
	new CheckPointArrow(this);

	// create camera for renderer
	// projection matrix (perspective FOV)
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(FIELD_O_VIEW, SCREEN_W, SCREEN_H, NEAR_PLANE_D, FAR_PLANE_D);
	mRenderer->SetProjectionMatrix(projection);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}

void Game::addBlock(Actor* actor) {
	my_blocks.emplace_back(actor);
}

void Game::removeBlock(Actor* actor) {
	auto iter = std::find(my_blocks.begin(), my_blocks.end(), actor);
	if (iter != my_blocks.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = my_blocks.end() - 1;
		std::iter_swap(iter, iter2);
		my_blocks.pop_back();
	}
}

void Game::addCheckpoint(CheckPoint* cp) {
	// first checkpoint is active
	if (my_checkpoints.empty())
		cp->setActive(true);
	my_checkpoints.push(cp);
}

void Game::popCheckpoint() {
	if (my_checkpoints.empty())
		return;
	// remove front checkpoint and activate next checkpoint
	CheckPoint* tmp = my_checkpoints.front();
	my_checkpoints.pop();
	delete tmp;
	// activate next checkpoint
	if (my_checkpoints.empty())
		return;
	my_checkpoints.front()->setActive(true);
}

CheckPoint* Game::getActiveCheckPoint() {
	if (my_checkpoints.empty())
		return nullptr;
	return my_checkpoints.front();
}

void Game::addCoin() {
	// increase count
	coin_count++;
	// update player's HUD
	if (my_player)
		my_player->GetComponent<HUD>()->setCoins(coin_count);
}

void Game::loadNextLevel() {
	// clear all actors
	while (!mActors.empty()) {
		delete mActors.back();
	}
	// clear checkpoint queue
	while (!my_checkpoints.empty())
		my_checkpoints.pop();
	// load next level
	LevelLoader::Load(this, next_level);
	// create new arrow
	new CheckPointArrow(this);
	// reset next_level string
	next_level = "";
}

void Game::removeCam(SecurityCam* in) {
	auto iter = std::find(my_cams.begin(), my_cams.end(), in);
	if (iter != my_cams.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = my_cams.end() - 1;
		std::iter_swap(iter, iter2);
		my_cams.pop_back();
	}
}