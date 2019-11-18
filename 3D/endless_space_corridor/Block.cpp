#include "Block.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "Player.h"
#include "Renderer.h"

class Mesh;

Block::Block(Game* owner, float x, float y, float z, bool explosive) : Actor(owner) {
	owner->addBlock(this);
	mScale = 25.f;
	my_isExplosive = explosive;
	// set given position
	mPosition = Vector3(x, y, z);
	// create collision box
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(1, 1, 1);
	// create mesh
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	if (my_isExplosive)
		my_meshc->SetTextureIndex(4);
	else
		my_meshc->SetTextureIndex(2);
}

Block::~Block() {
	mGame->removeBlock(this);
}

void Block::OnUpdate(float deltaTime) {
	if (mGame->getPlayer()->GetPosition().x > mPosition.x + 500)
		mState = ActorState::Destroy;
}

void Block::explode() {
	mState = ActorState::Destroy;
	for (auto x : mGame->getBlocks()) {
		if (x->GetState() == ActorState::Destroy)
			continue;
		Vector3 radius = mPosition - x->GetPosition();
		if (radius.LengthSq() <= 50 * 50) {
			if (x->isExplosive())
				x->explode();
			else
				x->SetState(ActorState::Destroy);
		}
	}
}
