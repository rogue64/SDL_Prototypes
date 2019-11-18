#include "Bullet.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "constants.h"
#include "Block.h"
#include "SDL/SDL_mixer.h"

class Mesh;

Bullet::Bullet(Game* owner, Vector3 pos, float speed) : Actor(owner) {
	mScale = 0.5f;
	mPosition = pos;
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
	my_mc = new MoveComponent(this);
	my_mc->SetForwardSpeed(speed);
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(10, 10, 10);
}


Bullet::~Bullet() {
}

void Bullet::OnUpdate(float deltaTime) {
	// destroy self after a second of existing
	death_timer += SEC * deltaTime;
	if (death_timer > SEC)
		mState = ActorState::Destroy;
	bool exploded = false;
	// check collisions with all blocks to see if bullet hits them
	for (auto x : mGame->getBlocks()) {
		if (my_cc->Intersect(x->getCollisionComp())) {
			// if the block is explosive, it explodes
			if (x->isExplosive()) {
				x->explode();
				// play explosion sound if its the first block exploding
				if (!exploded)
					Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
				exploded = true;
			}
			// else just destroy regular blocks
			else
				x->SetState(ActorState::Destroy);
		}
	}
}
