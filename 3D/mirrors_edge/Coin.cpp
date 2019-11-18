#include "Coin.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.h"

class Mesh;

Coin::Coin(Game* owner) : Actor(owner) {
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(100, 100, 100);
}

Coin::~Coin() {

}

void Coin::OnUpdate(float deltaTime) {
	mRotation += Math::Pi * deltaTime;
	if (my_cc->Intersect(mGame->getPlayer()->GetComponent<CollisionComponent>())) {
		// add coin to game's counter
		mGame->addCoin();
		// destroy self
		mState = ActorState::Destroy;
	}
}