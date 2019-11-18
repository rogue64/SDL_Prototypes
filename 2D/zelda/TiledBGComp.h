#pragma once
#include "constants.h"
#include "SpriteComponent.h"
#include <string>
#include <vector>

class TiledBGComp : public SpriteComponent {
public:
	TiledBGComp(class Actor* owner, int draw_dist = 50);
	~TiledBGComp();

	// set the tilemap with the given CSV file
	void loadTileCSV(const std::string& filename, int tileWidth, int tileHeight);

	void Draw(SDL_Renderer* renderer) override;

protected:
	// width of each tile in tilemap
	int tile_w = 0;
	// height of each tile in tilemap
	int tile_h = 0;
	// grid to hold the number representation of the tilemap
	std::vector< std::vector<int> > my_tmap;

	// function that returns the source tile wanted from tilemap
	SDL_Rect getSourceRect(int in);

	// function that returns the destination tile from coordinates
	SDL_Rect getDestRect(int row, int col);
};

