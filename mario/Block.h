#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

namespace block_const {
	static const int SIZE = 32;
};

class Block : public Actor {
public:
	Block(Game* owner, Vector2 pos, SDL_Texture* tex);
	~Block();
	// changes Block's texture to given texture; on failure, it keeps current texture
	void applyTexture(std::string in);
protected:
	// sprite component
	SpriteComponent* my_spr = nullptr;
	// collsion compoent
	CollisionComponent* my_cc = nullptr;
};

