#include "Player.h"

Player::Player(Game* owner, Vector2 pos) : Actor(owner) { 
	// set sprite component
	my_spr = new SpriteComponent(this, 200);
	my_spr->SetTexture(owner->getTexture("Assets/Mario/Idle.png"));
	// set collision component
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(plyr_const::SIZE, plyr_const::SIZE);
	// set players move component
	my_mc = new PlayerMovComp(this);
	mPosition = pos;
}

Player::~Player() { }
