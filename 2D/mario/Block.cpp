#include "Block.h"

Block::Block(Game* owner, Vector2 pos, SDL_Texture* tex) : Actor(owner) {
	my_spr = new SpriteComponent(this);
	my_spr->SetTexture(tex);
	mPosition = pos;
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(block_const::SIZE, block_const::SIZE);
	owner->addBlock(this);
}

Block::~Block() { mGame->removeBlock(this); }

void Block::applyTexture(std::string in) {
	// check if there os a new tex at given filename
	SDL_Texture* new_tex = mGame->getTexture(in);
	// if there is none, then just return quietly
	if (!new_tex)
		return;
	my_spr->SetTexture(new_tex);
}