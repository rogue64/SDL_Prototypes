#include "PlayerMovComp.h"
#include "Game.h"
#include "CollisionComponent.h"

PlayerMovComp::PlayerMovComp(Actor* owner) : MoveComponent(owner) { }


PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// change position due to velocity
	mOwner->SetPosition(mOwner->GetPosition() + (my_vel * deltaTime));

	CollisionComponent* player_cc = mOwner->GetComponent<CollisionComponent>();
	// check all collisions with game's doors
	const std::vector<Door*> game_doors = mOwner->GetGame()->getDoors();
	for (size_t i = 0; i < game_doors.size(); i++) {
		const Door* curr_door = game_doors[i];
		const Direction p_dir = ((Player*) mOwner)->getDirection();
		// if player intersects open door and has same direction, then go through door
		if ((curr_door->getState() == DoorState::Open)
			&& (p_dir == curr_door->getDirection())
			&& (player_cc->Intersect(curr_door->getCC()))) {
			switch (p_dir) {
				case (Direction::Left):
					mOwner->SetPosition(mOwner->GetPosition() - (Vector2(TILE_SIZE, 0) * 4));
					break;
				case (Direction::Right):
					mOwner->SetPosition(mOwner->GetPosition() + (Vector2(TILE_SIZE, 0) * 4));
					break;
				case (Direction::Up):
					mOwner->SetPosition(mOwner->GetPosition() - (Vector2(0, TILE_SIZE) * 4));
					break;
				case (Direction::Down):
					mOwner->SetPosition(mOwner->GetPosition() + (Vector2(0, TILE_SIZE) * 4));
					break;
			}
			// set new room of player
			((Player*) mOwner)->setRoom(curr_door->getDest());
			// (STRETCH POLISH) make function to pause game and pan camera over to new room; also make the camera depend on room instead of player
		}
	}

	// check collisions with game's secrets
	const std::vector<Secret*> game_secrets = mOwner->GetGame()->getSecrets();
	for (size_t i = 0; i < game_secrets.size(); i++) { 
		Secret* curr_secret = game_secrets[i];
		if (player_cc->Intersect(curr_secret->getCC())) { 
			// move player out of secret
			Vector2 offset;
			curr_secret->getCC()->GetMinOverlap(player_cc, offset);
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			// move secret if player hit it from correct side
			if (((Player*) mOwner)->getDirection() == curr_secret->getDir())
				curr_secret->moveSecret(deltaTime);
		}
	}

	// check collisions with game's colliders
	const std::vector<const Collider*> game_colliders = mOwner->GetGame()->getColliders();
	for (size_t i = 0; i < game_colliders.size(); i++) {
		// if player intersects a collider
		if (player_cc->Intersect(game_colliders[i]->getCC())) {
			// slow player animation (STRETCH POLISH)
			// move player out of collider
			Vector2 offset;
			game_colliders[i]->getCC()->GetMinOverlap(player_cc, offset);
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	// update the camera's position (keeping player at center-screen)
	mOwner->GetGame()->getCamera().x = mOwner->GetPosition().x - (WINDOW_W / 2);
	mOwner->GetGame()->getCamera().y = mOwner->GetPosition().y - (WINDOW_H / 2);
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// set horizontal and verticle speed depending on input
	my_vel.x = (keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]) * player_const::M_SPEED;
	my_vel.y = (keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP]) * player_const::M_SPEED;
}