#include "PlayerMovComp.h"
#include "Game.h"

PlayerMovComp::PlayerMovComp(Actor* owner) : MoveComponent(owner) { }


PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// use discrete-Euler integration to move player over deltaTime timestep

	// change velocity due to acceleration/force
	// apply gravity
	my_ySpeed += GRAV * deltaTime;
	
	// change position due to velocity; don't allow to move off left of screen
	mOwner->SetPosition(Vector2(Math::Clamp(mOwner->GetPosition().x, mOwner->GetGame()->getCamera().x,
								mOwner->GetGame()->getCamera().x + WINDOW_W) + (GetForwardSpeed() * deltaTime),
								mOwner->GetPosition().y + (my_ySpeed * deltaTime)));

	// check to see if colliding with all blocks in game
	const std::vector<Block*> g_blocks = mOwner->GetGame()->getBlocks();
	const CollisionComponent* p_collision = ((Player*) mOwner)->getCollsion();
	bool hit_block = false;
	for (size_t i = 0; i < g_blocks.size(); i++) { 
		Vector2 offset;
		CollSide collision;
		collision = g_blocks[i]->getCollision()->GetMinOverlap(p_collision, offset);
		// if we collide, fix position using offset
		if (collision != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			hit_block = true;
		}
		// if we collide on top of a block, stop falling
		// QUESTION: why does lab notes ask us to only do this when my_ySpeed > 0? we won't ever land by going up
		if (collision == CollSide::Top) {
			my_ySpeed = 0;
			in_air = false;
		}
		if (collision == CollSide::Bottom) {
			my_ySpeed = 0;
		}
	}
	// after checking each block, see if any were hit; this way we know if we are in the air
	if (!in_air) // without with check, you are able to jump after hitting side of wall
		in_air = !hit_block;

	// stop falling at bottom of world
	if (mOwner->GetPosition().y > WORLD_BOTTOM) { 
		mOwner->SetPosition(Vector2(mOwner->GetPosition().x, WORLD_BOTTOM));
		my_ySpeed = 0;
		in_air = false;
	}

	// update the camera's position (only x-position and to the right)
	mOwner->GetGame()->getCamera().x = Math::Clamp(mOwner->GetPosition().x - (WINDOW_W / 2),
									mOwner->GetGame()->getCamera().x, mOwner->GetPosition().x + (WINDOW_W / 2));
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// set horizontal speed depending on input
	SetForwardSpeed((keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]) * player_const::M_SPEED);
	
	// set verticle speed depending on jump input
	if (!jump_pressed && !in_air && keyState[SDL_SCANCODE_SPACE]) {
		my_ySpeed = -player_const::J_SPEED;
		in_air = true; // might not be needed here, but for safety
	}
	// update jump_pressed
	jump_pressed = (bool) keyState[SDL_SCANCODE_SPACE];
}