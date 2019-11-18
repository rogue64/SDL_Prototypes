#include "Block.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"

class Mesh;

Block::Block(Game* owner) : Actor (owner) {
	owner->addBlock(this);
	mScale = 64.f;
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(1, 1, 1);
}


Block::~Block() {
	mGame->removeBlock(this);
}
