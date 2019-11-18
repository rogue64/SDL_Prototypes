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
	my_window = SDL_CreateWindow("It's a me, Lab4", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0/*SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_ALLOW_HIGHDPI*/);

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

void Game::addBlock(Block* block) { 
	my_blocks.push_back(block);
}

void Game::removeBlock(Block* block) { 
	std::vector<Block*>::iterator i = my_blocks.begin();
	for (; i != my_blocks.end(); i++)
		if (*i == block)
			break;
	if (i != my_blocks.end())
		my_blocks.erase(i);
}

void Game::LoadData() {
	// load background
	Actor* Background = new Actor(this);
	SpriteComponent* bg_spr = new SpriteComponent(Background, 1);
	bg_spr->SetTexture(getTexture("Assets/Background.png"));
	Background->SetPosition(Vector2(bg_spr->GetTexWidth() / 2, bg_spr->GetTexHeight() / 2)); // needs to be to this position since bg is so big

	// parse level file
	std::ifstream level("Assets/Level1.txt");
	std::string curr_line;
	size_t i = 0;
	while (std::getline(level, curr_line)) {
		for (size_t j = 0; j < curr_line.size(); j++) {
			// get position of current cell
			Vector2 curr_pos((j * block_const::SIZE) + (block_const::SIZE / 2),
				(i * block_const::SIZE) + (block_const::SIZE / 2));
			if (((int)curr_line[j] >= block_const::FIRST_TYP) && 
				((int)curr_line[j] < block_const::FIRST_TYP + block_const::NUM_TYPS)) {
				std::string block_name = "Assets/Block";
				block_name += curr_line[j];
				block_name += ".png"; // workaround to use 3 lines, won't let me do in 1 or 2 lines
				new Block(this, curr_pos, getTexture(block_name));
			}
			if (curr_line[j] == 'P')
				new Player(this, curr_pos);
		}
		i++;
	}
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
