#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "constants.h"
#include "Bullet.h"
#include "SDL/SDL_mixer.h"

class Mesh;

Player::Player(Game* owner) : Actor(owner) {
	// set player's mesh
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(owner->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
	// create collision component
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(plyr_c::WIDTH, plyr_c::HEIGHT, plyr_c::DEPTH);
	// create player move component
	my_pmc = new PlayerMovComp(this);
}

Player::~Player() {
}

void Player::OnProcessInput(const Uint8* keyState) {
	// shoot bullet on rising edge of space-bar
	if (keyState[SDL_SCANCODE_SPACE] && !space_pressed) {
		new Bullet(mGame, mPosition, my_pmc->GetForwardSpeed() + 500);
		// play shooting sound
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Shoot.wav"), 0);
	}
	space_pressed = keyState[SDL_SCANCODE_SPACE];
}