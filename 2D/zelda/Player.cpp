#include "Player.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "PlayerMovComp.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Math.h"
#include "Collider.h"

Player::Player(Game* owner, Vector2 pos) : Actor(owner) { 
	// set collision component
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(player_const::SIZE, player_const::SIZE);
	// set players move component
	my_mc = new PlayerMovComp(this);
	mPosition = pos;
	// load player's animations
	my_spr = new AnimatedSprite(this, 150);
	std::vector<SDL_Texture*> curr_anim = {
		mGame->getTexture("Assets/Link/WalkUp0.png"),
		mGame->getTexture("Assets/Link/WalkUp1.png")
	};
	my_spr->AddAnimation("walkUp", curr_anim);
	curr_anim = {
		mGame->getTexture("Assets/Link/WalkDown0.png"),
		mGame->getTexture("Assets/Link/WalkDown1.png")
	};
	my_spr->AddAnimation("walkDown", curr_anim);
	curr_anim = {
		mGame->getTexture("Assets/Link/WalkRight0.png"),
		mGame->getTexture("Assets/Link/WalkRight1.png")
	};
	my_spr->AddAnimation("walkRight", curr_anim);
	curr_anim = {
		mGame->getTexture("Assets/Link/WalkLeft0.png"),
		mGame->getTexture("Assets/Link/WalkLeft1.png")
	};
	my_spr->AddAnimation("walkLeft", curr_anim);
	// set default animation and pause the animation
	my_spr->SetAnimation("walkLeft");
	my_spr->SetIsPaused(true);
}

void Player::OnProcessInput(const Uint8* keyState) { 
	// get input for the axis
	int horiz = keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT];
	int verti = keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP];
	// change animations due to input
	if ((keyState[SDL_SCANCODE_LEFT] && !leftPressed) || (horiz < 0 && verti == 0)) {
		my_spr->SetAnimation("walkLeft");
		my_dir = Direction::Left;
	}
	if ((keyState[SDL_SCANCODE_RIGHT] && !rightPressed) || (horiz > 0 && verti == 0)) {
		my_spr->SetAnimation("walkRight");
		my_dir = Direction::Right;
	}
	if ((keyState[SDL_SCANCODE_UP] && !upPressed) || (verti < 0 && horiz == 0)) {
		my_spr->SetAnimation("walkUp");
		my_dir = Direction::Up;
	}
	if ((keyState[SDL_SCANCODE_DOWN] && !downPressed) || (verti > 0 && horiz == 0)) {
		my_spr->SetAnimation("walkDown");
		my_dir = Direction::Down;
	}
	leftPressed = keyState[SDL_SCANCODE_LEFT];
	rightPressed = keyState[SDL_SCANCODE_RIGHT];
	upPressed = keyState[SDL_SCANCODE_UP];
	downPressed = keyState[SDL_SCANCODE_DOWN];
	if (!(leftPressed || rightPressed || upPressed || downPressed) || (verti == 0 && horiz == 0))
		my_spr->SetIsPaused(true);
	else
		my_spr->SetIsPaused(false);
}

void Player::OnUpdate(float deltaTime) { 
	/* MOVED TO PLAYER MOVEMENT COMP
	// do collision checks for game's colliders
	const std::vector<const Collider*> game_colliders = mGame->getColliders();
	for (size_t i = 0; i < game_colliders.size(); i++) {
		// if player intersects a collider
		if (my_cc->Intersect(game_colliders[i]->getCC())) { 
			// slow player animation (STRETCH)
			// move player out of collider
			Vector2 offset;
			game_colliders[i]->getCC()->GetMinOverlap(my_cc, offset);
			mPosition = mPosition + offset;
		}
	}*/
}

Player::~Player() { }
