#include "TiledBGComp.h"
#include "CSVHelper.h"
#include "Actor.h"
#include "Game.h"
#include <fstream>
#include <SDL\SDL_image.h>
#include "SDL\SDL.h"

TiledBGComp::TiledBGComp(class Actor* owner, int draw_dist) : SpriteComponent(owner, draw_dist) { }

TiledBGComp::~TiledBGComp() { }

void TiledBGComp::loadTileCSV(const std::string& filename, int tileWidth, int tileHeight) { 
	// set tile width and height
	tile_w = tileWidth;
	tile_h = tileHeight;
	// clear the tilemap before resetting it
	my_tmap.clear();
	// open up csv file
	std::ifstream file(filename);
	// parse each line to get tilemap
	std::string curr_line;
	while (std::getline(file, curr_line)) {
		std::vector<int> curr_ints;
		std::vector<std::string> tokens = CSVHelper::Split(curr_line);
		if (tokens.empty())
			continue;
		for (size_t i = 0; i < tokens.size(); i++)
			curr_ints.push_back(std::stoi(tokens[i]));
		my_tmap.push_back(curr_ints);
	}
}

void TiledBGComp::Draw(SDL_Renderer* renderer) {
	if (mTexture && !my_tmap.empty()) {
		for (size_t i = 0; i < my_tmap.size(); i++) {
			std::vector<int> curr_row = my_tmap[i];
			for (size_t j = 0; j < curr_row.size(); j++) {
				// -1 means that there is no tile there
				if (my_tmap[i][j] == -1)
					continue;
				// get the tile to be applied from the tile map
				SDL_Rect source_r = getSourceRect(my_tmap[i][j]);
				// get the current cell of the "tile grid" of the level
				SDL_Rect dest_r = getDestRect(i, j);
				// draw tile in it's position
				SDL_RenderCopyEx(renderer,
								 mTexture,
								 &source_r,
								 &dest_r,
								 -Math::ToDegrees(mOwner->GetRotation()),
								 nullptr,
								 SDL_FLIP_NONE);
			}
		}
		/*SDL_Rect r;
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner 
		// but proportional to the camera: screen_pos = world_pos - camera_pos
		r.x = static_cast<int>((mOwner->GetPosition().x - r.w / 2) - mOwner->GetGame()->getCamera().x);
		r.y = static_cast<int>((mOwner->GetPosition().y - r.h / 2) - mOwner->GetGame()->getCamera().y);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
						 mTexture,
						 nullptr,
						 &r,
						 -Math::ToDegrees(mOwner->GetRotation()),
						 nullptr,
						 SDL_FLIP_NONE);*/
	}
}

SDL_Rect TiledBGComp::getSourceRect(int in) {
	int rows = mTexHeight / tile_h;
	int cols = mTexWidth / tile_w;
	SDL_Rect out;
	out.x = (in % cols) * tile_w;
	out.y = (in / rows) * tile_h;
	out.w = tile_w;
	out.h = tile_h;
	return out;
}

SDL_Rect TiledBGComp::getDestRect(int row, int col) {
	SDL_Rect out = { 0, 0, 0, 0 };
	if (my_tmap.empty() || row > my_tmap.size() || col > my_tmap[0].size())
		return out;
	// make sure each tile is drawn in reference to the camera position
	out.x = (col * tile_w) - mOwner->GetGame()->getCamera().x;
	out.y = (row * tile_h) - mOwner->GetGame()->getCamera().y;
	out.w = tile_w;
	out.h = tile_h;
	return out;
}