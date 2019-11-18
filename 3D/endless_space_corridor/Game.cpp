//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "constants.h"
#include "Player.h"
#include "SideBlocks.h"
#include "Block.h"
#include "SDL/SDL_mixer.h"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// TODO: Create renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(SCREEN_W, SCREEN_H))
		return false;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
	
	spawnBlocks();

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
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

void Game::LoadData()
{
	// create player
	my_player = new Player(this);
	// create first 7 side blocks
	for (int i = 0; i < 7; i++) {
		new SideBlocks(this, i * 500, -500, 0, i + 1);
		new SideBlocks(this, i * 500, 500, 0, i + 1);
	}
	// create the first few block obstacles
	createBlocks(1, 1000);
	createBlocks(2, 2000);

	// create camera for renderer
	// projection matrix (perspective FOV)
	Matrix4 projection = Matrix4::CreatePerspectiveFOV(FIELD_O_VIEW, SCREEN_W, SCREEN_H, NEAR_PLANE_D, FAR_PLANE_D);
	mRenderer->SetProjectionMatrix(projection);
	// look at matrix; (behind and above origin, slightly ahead of origin, z-axis is up)
	Matrix4 lookAt = Matrix4::CreateLookAt(Vector3(-300, 0, 100), Vector3(20, 0, 0), Vector3::UnitZ);
	mRenderer->SetViewMatrix(lookAt);
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

void Game::AddActor(Actor* actor)
{
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

void Game::addBlock(Block* block) {
	my_blocks.push_back(block);
}

void Game::removeBlock(Block* block)
{
	auto iter = std::find(my_blocks.begin(), my_blocks.end(), block);
	if (iter != my_blocks.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = my_blocks.end() - 1;
		std::iter_swap(iter, iter2);
		my_blocks.pop_back();
	}
}

void Game::createBlocks(int file_i, float x) {
	// open file stream
	std::string filename = "Assets/Blocks/" + std::to_string(file_i) + ".txt";
	//filename += ".txt";
	std::ifstream file(filename);
	std::string curr_line;
	int i = 0;
	// set left boundary
	float left = -237.f;
	// set top boundary
	float top = 237.f;
	// use characters from file to wee where to place blocks
	while (file) {
		std::getline(file, curr_line);
		for (int j = 0; j < curr_line.length(); j++) {
			if (curr_line[j] == 'A')
				new Block(this, x, left + (j * 25), top - (i * 25), false);
			else if (curr_line[j] == 'B')
				new Block(this, x, left + (j * 25), top - (i * 25), true);
		}
		i++;
	}
	// close file stream
	file.close();
}

void Game::spawnBlocks() {
	int curr_thou = ((int) my_player->GetPosition().x) / 1000;
	if (curr_thou > thou_count) {
		thou_count++;
		obstacle_index++;
		if (obstacle_index > 20) {
			int rand_index = Random::GetIntRange(1, 20);
			createBlocks(rand_index, (thou_count + 3) * 1000);
		}
		else
			createBlocks(obstacle_index, (thou_count + 3) * 1000);
	}
}