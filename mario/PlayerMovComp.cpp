#include "PlayerMovComp.h"
#include "Game.h"

PlayerMovComp::PlayerMovComp(Actor* owner) : MoveComponent(owner) { }


PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// move player horizontally at rate of forward speed
	mOwner->SetPosition(Vector2(mOwner->GetPosition().x + (GetForwardSpeed() * deltaTime), mOwner->GetPosition().y));
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// set speed depending on speed
	SetForwardSpeed((keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]) * PLAYER_SPD);
}