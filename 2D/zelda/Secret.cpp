#include "Secret.h"
#include "CollisionComponent.h"
#include "Game.h"

Secret::Secret(Game* owner, int x, int y, int w, int h, std::string dir, 
			   std::string room) : Actor(owner) {
	// add secret to game
	owner->addSecret(this);
	
	// set collider size and width
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(w, h);
	mPosition = Vector2((w / 2) + x, (h / 2) + y);

	// set sprite for the secret
	my_spr = new SpriteComponent(this);
	my_spr->SetTexture(mGame->getTexture("Assets/SecretBlock.png"));

	// set room
	my_room = room;
	// set start pos
	start_pos = mPosition;
	// set direction
	if (dir == "Left")
		my_dir = Direction::Right;
	else if (dir == "Right")
		my_dir = Direction::Left;
	else if (dir == "Down")
		my_dir = Direction::Up;
	else
		my_dir = Direction::Down;
}

Secret::~Secret() { mGame->removeSecret(this); }

void Secret::moveSecret(float deltaTime) { 
	// return if the room is already unlocked
	if (unlocked)
		return;
	
	// vector to store how much the block should move
	Vector2 offset = Vector2::Zero;

	// move vector depending on direction
	switch (my_dir) {
		case (Direction::Left) :
			offset.x = -player_const::M_SPEED / 2;
			break;
		case (Direction::Right) :
			offset.x = player_const::M_SPEED / 2;
			break;
		case (Direction::Up) :
			offset.y = -player_const::M_SPEED / 2;
			break;
		case (Direction::Down) :
			offset.y = player_const::M_SPEED / 2;
			break;
	}

	mPosition = mPosition + offset * deltaTime;

	// if distance moved enough, then unlock doors in room
	Vector2 dist = mPosition - start_pos;
	if (dist.LengthSq() > max_dist * max_dist) {
		mGame->openDoors(my_room);
		unlocked = true;
	}
}