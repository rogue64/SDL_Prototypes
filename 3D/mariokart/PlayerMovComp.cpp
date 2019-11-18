#include "PlayerMovComp.h"
#include "constants.h"
#include "SDL\SDL_scancode.h"
#include "Math.h"
#include "Game.h"
#include "Renderer.h"
#include "HeightMap.h"

PlayerMovComp::PlayerMovComp(Player* owner) : VehicleMovComp((Actor*) owner) {
	mOwner->SetPosition(mOwner->GetGame()->getHeightMap()->cellToWorld(39, 58));
	//Vector2 test = mOwner->GetGame()->getHeightMap()->worldToCell(mOwner->GetPosition().y, mOwner->GetPosition().x);
}

PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// call superclass update first
	VehicleMovComp::Update(deltaTime);
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// check accelleration pedal
	bool curr_pedal = false;
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_RETURN])
		curr_pedal = true;
	setPedalHeld(curr_pedal);
	// check directional input
	int curr_input = 0;
	curr_input = (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
				- (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]);
	setDirInput(curr_input);
}