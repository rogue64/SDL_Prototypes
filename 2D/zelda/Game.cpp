#include "Game.h"

Game::Game() {
	
}

bool Game::initialize() {
	// intialize sdl and error check
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// create sdl window and store its pointer
	my_window = SDL_CreateWindow("Lab5 may be dangerous.. Take this!", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);

	// error check
	if (!my_window) {
		SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	// create sdl renderer and store its pointer
	my_renderer = SDL_CreateRenderer(my_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// initialize SDL image; QUESTION: why does this have to be done after the renderer is initialized??
	// supposed to return bitmap with supported flags upon succes
	if (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG != IMG_INIT_PNG) {
		SDL_Log("Unable to initialize IMG_Init with PNG support.\n");
		return false;
	}

	// initialize sound
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) { 
		SDL_Log("Unable to initialize Mix_OpenAudio: %s", SDL_GetError());
		return false;
	}

	// error check
	if (!my_renderer) {
		SDL_Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	// initialize randomization
	Random::Init();

	// load initial data and objects
	LoadData();

	// initialize SDL_GetTicks(); always at end
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
	// quit sound support
	Mix_CloseAudio();

	// quit image suppport
	IMG_Quit();

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

	// process input for all actors
	for (size_t i = 0; i < my_actors.size(); i++)
		my_actors[i]->ProcessInput(state);
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

	// actors to be destroyed; so that they aren't deleted as we iterate my_actors
	std::vector<Actor*> ded_actors;

	// go through active actors and update them
	for (size_t i = 0; i < my_actors.size(); i++) {
		if (my_actors[i]->GetState() == my_actorstate::Active)
			my_actors[i]->Update(delt_time);
		// add a destroyed actor to ded_actors
		else if (my_actors[i]->GetState() == my_actorstate::Destroy)
			ded_actors.push_back(my_actors[i]);
	}

	// no go through and delete all actors in ded_actors
	for (size_t i = 0; i < ded_actors.size(); i++)
		delete ded_actors[i];

	// add any newly made actors to my_actors
	// QUESTION: should this be before the actor update loop?
	for (size_t i = 0; i < new_actors.size(); i++)
		my_actors.push_back(new_actors[i]);
	new_actors.clear();
}

void Game::generateOutput() {
	// clear back buffer; w/ error check
	if (SDL_SetRenderDrawColor(my_renderer, 0, 0, 0, 255) < 0) {
		SDL_Log("Unable to set renderer color: %s", SDL_GetError());
		off = true;
		return;
	}
	if (SDL_RenderClear(my_renderer) < 0) {
		SDL_Log("Unable to clear renderer: %s", SDL_GetError());
		off = true;
		return;
	}

	// draw game objects' SpriteComponents
	for (size_t i = 0; i < my_sprites.size(); i++) {
		if (my_sprites[i]->IsVisible())
			my_sprites[i]->Draw(my_renderer);
	}

	// present
	SDL_RenderPresent(my_renderer);
}

void Game::addActor(Actor* actor) {
	new_actors.push_back(actor);
}

void Game::removeActor(Actor* actor) {
	std::vector<Actor*>::iterator i = my_actors.begin();
	for (; i != my_actors.end(); i++)
		if (*i == actor)
			break;
	if (i != my_actors.end())
		my_actors.erase(i);
}

void Game::addCollider(Collider* collider) { 
	my_colliders.push_back(collider);
}

void Game::removeCollider(Collider* collider) { 
	std::vector<const Collider*>::iterator i = my_colliders.begin();
	for (; i != my_colliders.end(); i++)
		if (*i == collider)
			break;
	if (i != my_colliders.end())
		my_colliders.erase(i);
}

void Game::addDoor(Door* door) { 
	my_doors.push_back(door);
}

void Game::removeDoor(Door* door) { 
	std::vector<Door*>::iterator i = my_doors.begin();
	for (; i != my_doors.end(); i++)
		if (*i == door)
			break;
	if (i != my_doors.end())
		my_doors.erase(i);
}

void Game::parseCSV(const std::string& filename, const std::string& room) {
	std::ifstream file(filename);
	std::string curr_line;
	while (std::getline(file, curr_line)) { 
		std::vector<std::string> tokens;
		tokens = CSVHelper::Split(curr_line);
		// create colliders
		if (tokens[0] == "Collider") { 
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);
			int w = std::stoi(tokens[3]);
			int h = std::stoi(tokens[4]);
			new Collider(this, x, y, w, h);
		}
		// create player
		else if (tokens[0] == "Player") {
			// only one player at a time
			if (my_player)
				delete my_player;
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);
			my_player = new Player(this, Vector2(x, y));
		}
		// create door
		else if (tokens[0] == "Door") { 
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);
			int w = std::stoi(tokens[3]);
			int h = std::stoi(tokens[4]);
			new Door(this, x, y, w, h, tokens[5], tokens[6], tokens[7], room);
		}
		// create secret blocks
		else if (tokens[0] == "SecretBlock") { 
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);
			int w = std::stoi(tokens[3]);
			int h = std::stoi(tokens[4]);
			std::string dir = tokens[6];
			new Secret(this, x, y, w, h, tokens[6], room);
		}
	}
}

void Game::LoadData() {
	// create game's player in the center of the screen
	//my_player = new Player(this, Vector2(WINDOW_W / 2, WINDOW_H / 2));

	// create background tilemap
	Actor* bgMap = new Actor(this);
	TiledBGComp* bgMap_comp = new TiledBGComp(bgMap);
	bgMap_comp->SetTexture(getTexture("Assets/Dungeon/DungeonTiles.png"));
	bgMap_comp->loadTileCSV("Assets/Dungeon/DungeonMapBG.csv", TILE_SIZE, TILE_SIZE);

	// load objects from CSV files
	parseCSV("Assets/Dungeon/Room1.csv", "Room1");
	parseCSV("Assets/Dungeon/Room2.csv", "Room2");
	parseCSV("Assets/Dungeon/Room3.csv", "Room3");
	parseCSV("Assets/Dungeon/Room4.csv", "Room4");
	parseCSV("Assets/Dungeon/Room5.csv", "Room5");
	parseCSV("Assets/Dungeon/Room6.csv", "Room6");
	parseCSV("Assets/Dungeon/Room7.csv", "Room7");

}

void Game::UnloadData() {
	// destroy all actors
	while (!my_actors.empty())
		delete my_actors.back();

	// destroy all textures and clear textureMap
	for (auto x : textureMap)
		SDL_DestroyTexture(x.second);
	textureMap.clear();
}

SDL_Texture* Game::getTexture(std::string filename) {
	// if texture isn't already stored in textureMap then SDL_Load() it
	if (textureMap.find(filename) == textureMap.end()) {
		// create surface from given file
		SDL_Surface* tmp_surf = nullptr;
		if (!(tmp_surf = IMG_Load(filename.c_str()))) {
			SDL_Log("getTexture error: File %s either doesn't exist or isn't a texture.\n", filename);
			return nullptr;
		}
		// create texture from surface
		SDL_Texture* tmp_text = nullptr;
		if (!(tmp_text = SDL_CreateTextureFromSurface(my_renderer, tmp_surf))) {
			SDL_Log("getTexture error: %s\n", SDL_GetError());
			SDL_FreeSurface(tmp_surf);
			return nullptr;
		}

		// free surface
		SDL_FreeSurface(tmp_surf);

		// emplace texture in textureMap
		textureMap.emplace(filename, tmp_text);
	}

	// return texture
	return textureMap.find(filename)->second;
}

Mix_Chunk* Game::getSound(const std::string& filename) { 
	// if mix chunk isn't already stored in soundMap then Mix_LoadWAV() it
	if (soundMap.find(filename) == soundMap.end()) {
		Mix_Chunk* tmp_chunk = nullptr;
		if (!(tmp_chunk = Mix_LoadWAV(filename.c_str()))) {
			SDL_Log("getSound error: %s\n", SDL_GetError());
			return nullptr;
		}
		// emplace Mix Chunk in soundMap
		soundMap.emplace(filename, tmp_chunk);
	}

	// return Mix Chunk
	return soundMap.find(filename)->second;
}

void Game::addSprite(SpriteComponent* sprite) {
	my_sprites.push_back(sprite);

	// sort sprites by draw-order
	std::sort(my_sprites.begin(), my_sprites.end(), [](SpriteComponent* a, SpriteComponent* b)
		{ return a->GetDrawOrder() < b->GetDrawOrder(); });
}

void Game::removeSprite(SpriteComponent* sprite) {
	std::vector<SpriteComponent*>::iterator i = my_sprites.begin();
	for (; i != my_sprites.end(); i++)
		if (*i == sprite)
			break;
	if (i != my_sprites.end())
		my_sprites.erase(i);
}

void Game::addSecret(Secret* secret) { 
	my_secrets.push_back(secret);
}

void Game::removeSecret(Secret* secret) { 
	std::vector<Secret*>::iterator i = my_secrets.begin();
	for (; i != my_secrets.end(); i++)
		if (*i == secret)
			break;
	if (i != my_secrets.end())
		my_secrets.erase(i);
}

void Game::openDoors(std::string room) { 
	for (size_t i = 0; i < my_doors.size(); i++) {
		if (my_doors[i]->getRoom() == room) {
			my_doors[i]->setState(DoorState::Open);
			my_doors[i]->updateAnimation();
		}
	}
}
