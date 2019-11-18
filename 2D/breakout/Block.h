#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"

class Block : public Actor {
public:
	Block(Game* owner, Vector2 pos, SDL_Texture* tex);
	~Block();
	// changes Block's texture to given texture; on failure, it keeps current texture
	void applyTexture(std::string in);
protected:
	// sprite component
	SpriteComponent* my_spr = nullptr;
};

