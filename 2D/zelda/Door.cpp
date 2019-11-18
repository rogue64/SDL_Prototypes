#include "Door.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"

Door::Door(Game* owner, int x, int y, int w, int h, std::string dest,
		   std::string dir, std::string state, std::string room) : Actor(owner) {
	// add self to game's door vector
	owner->addDoor(this);

	// create collision component and set position
	my_cc = new CollisionComponent(this);
	//my_cc->SetSize(w, h);
	mPosition = Vector2((w / 2) + x, (h / 2) + y);

	// set destination
	destination = dest;
	
	// set state
	if (state == "Locked")
		my_state = DoorState::Locked;
	else if (state == "Closed")
		my_state = DoorState::Closed;
	else
		state = "Open";

	// set direction
	if (dir == "Left")
		my_dir = Direction::Left;
	else if (dir == "Right")
		my_dir = Direction::Right;
	else if (dir == "Down")
		my_dir = Direction::Down;
	else
		my_dir = Direction::Up;

	// set room
	my_room = room;

	// set collision dimensions based on direction
	if (my_dir == Direction::Up || my_dir == Direction::Down)
		my_cc->SetSize(30, 64);
	else
		my_cc->SetSize(64, 30);

	// create animation component and load animations
	my_spr = new AnimatedSprite(this);
	std::vector<SDL_Texture*> curr_anim = { mGame->getTexture("Assets/Door/UpLocked.png") };
	my_spr->AddAnimation("upLocked", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/DownLocked.png") };
	my_spr->AddAnimation("downLocked", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/LeftLocked.png") };
	my_spr->AddAnimation("leftLocked", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/RightLocked.png") };
	my_spr->AddAnimation("rightLocked", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/UpOpen.png") };
	my_spr->AddAnimation("upOpen", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/DownOpen.png") };
	my_spr->AddAnimation("downOpen", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/LeftOpen.png") };
	my_spr->AddAnimation("leftOpen", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/RightOpen.png") };
	my_spr->AddAnimation("rightOpen", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/UpClosed.png") };
	my_spr->AddAnimation("upClosed", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/DownClosed.png") };
	my_spr->AddAnimation("downClosed", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/LeftClosed.png") };
	my_spr->AddAnimation("leftClosed", curr_anim);
	curr_anim = { mGame->getTexture("Assets/Door/RightClosed.png") };
	my_spr->AddAnimation("rightClosed", curr_anim);

	// DEBUGGING/TESTING: set all doors to open
	//my_state = DoorState::Open;

	// set animation based on state
	this->updateAnimation();
}

Door::~Door() { mGame->removeDoor(this); }

void Door::updateAnimation() { 
	switch (my_dir) {
		case (Direction::Up): {
			switch (my_state) { 
				case (DoorState::Open):
					my_spr->SetAnimation("upOpen");
					break;
				case (DoorState::Closed):
					my_spr->SetAnimation("upClosed");
					break;
				case (DoorState::Locked):
					my_spr->SetAnimation("upLocked");
					break;
			}
			break;
		}
		case (Direction::Down): {
			switch (my_state) {
				case (DoorState::Open):
					my_spr->SetAnimation("downOpen");
					break;
				case (DoorState::Closed):
					my_spr->SetAnimation("downClosed");
					break;
				case (DoorState::Locked):
					my_spr->SetAnimation("downLocked");
					break;
			}
			break;
		}
		case (Direction::Left): {
			switch (my_state) {
				case (DoorState::Open):
					my_spr->SetAnimation("leftOpen");
					break;
				case (DoorState::Closed):
					my_spr->SetAnimation("leftClosed");
					break;
				case (DoorState::Locked):
					my_spr->SetAnimation("leftLocked");
					break;
			}
			break;
		}
		case (Direction::Right): {
			switch (my_state) {
				case (DoorState::Open):
					my_spr->SetAnimation("rightOpen");
					break;
				case (DoorState::Closed):
					my_spr->SetAnimation("rightClosed");
					break;
				case (DoorState::Locked):
					my_spr->SetAnimation("rightLocked");
					break;
			}
			break;
		}
	}
}
