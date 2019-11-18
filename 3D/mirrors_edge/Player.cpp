#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "constants.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "HUD.h"


Player::Player(Game* owner) : Actor(owner) {
	mScale = 1.f;
	// create camera component
	my_camc = new CameraComponent((Actor*) this);
	// create collision component
	my_cc = new CollisionComponent((Actor*) this);
	my_cc->SetSize(50, 175, 50);
	// create player move component; NOTE: has to come after camera comp since it needs to exist before the movement
	my_pmc = new PlayerMovComp(this);
	// create HUD component
	my_hud = new HUD(this);
}

Player::~Player() {
}

void Player::OnProcessInput(const Uint8* keyState) {

}

void Player::OnUpdate(float deltaTime) {
	CheckPoint* active_cp = mGame->getActiveCheckPoint();
	// if colliding with the active checkpoint, set no respawn point ant activate next checkpoint
	if (active_cp && my_cc->Intersect(active_cp->GetComponent<CollisionComponent>())) {
		setRespawnPosition(active_cp->GetPosition());
		// set checkpoint's message to display on hud
		my_hud->setCPMessage(active_cp->getText());
		mGame->popCheckpoint();
	}
}