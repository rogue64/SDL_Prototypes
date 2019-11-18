#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "constants.h"
#include "CameraComponent.h";

class Mesh;

Player::Player(Game* owner) : Actor(owner) {
	mScale = 0.75f;
	// set player's mesh
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(owner->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	// create player move component
	my_pmc = new PlayerMovComp(this);
	// create camera component
	my_camc = new CameraComponent((Actor*) this);
}

Player::~Player() {
}

void Player::OnProcessInput(const Uint8* keyState) {
	// shoot bullet on rising edge of space-bar

}