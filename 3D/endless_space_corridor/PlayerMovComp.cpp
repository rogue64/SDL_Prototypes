#include "PlayerMovComp.h"
#include "constants.h"
#include "SDL\SDL_scancode.h"
#include "Math.h"
#include "Game.h"
#include "Renderer.h"
#include "Block.h"

PlayerMovComp::PlayerMovComp(Player* owner) : MoveComponent((Actor*) owner) {
	SetForwardSpeed(plyr_c::FORWRD_SPD);
	// start your engines
	engine_channel = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipLoop.wav"), 1);
}

PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// move player position based on the input and auto forward velocity
	Vector3 new_pos = mOwner->GetPosition();
	new_pos.x += GetForwardSpeed() * deltaTime;
	new_pos.y = Math::Clamp(new_pos.y + (horz_input * plyr_c::STEER_SPD * speed_mult * deltaTime), -plyr_c::HORZ_LIM, plyr_c::HORZ_LIM);
	new_pos.z = Math::Clamp(new_pos.z + (vert_input * plyr_c::STEER_SPD * speed_mult * deltaTime), -plyr_c::VERT_LIM, plyr_c::VERT_LIM);
	mOwner->SetPosition(new_pos);

	// check if player dies
	for (auto x : mOwner->GetGame()->getBlocks())
		if (((Player*)mOwner)->getCollisionComp()->Intersect(x->getCollisionComp())) {
			mOwner->SetState(ActorState::Paused);
			// play dying sound and stop engine
			Mix_PlayChannel(engine_channel, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
			break;
		}

	// speed up movement
	if (speed_timer > 10 * SEC) {
		speed_timer = 0;
		speed_mult += 0.15f;
		SetForwardSpeed(GetForwardSpeed() * speed_mult);
	}
	speed_timer += SEC * deltaTime;

	// set camera view position based on player position
	Vector3 eye_pos = new_pos;
	eye_pos.x -= HORZ_D;
	eye_pos.z += VERT_D;
	Vector3 targ_pos = new_pos;
	targ_pos.x += TARG_D;
	Matrix4 camera_view = Matrix4::CreateLookAt(eye_pos, targ_pos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera_view);
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// get input for steering ship up, down, left, and right
	vert_input = keyState[SDL_SCANCODE_W] - keyState[SDL_SCANCODE_S];
	horz_input = keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A];
}