#include "CheckPoint.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

class Mesh;

CheckPoint::CheckPoint(Game* owner) : Actor(owner) {
	my_meshc = new MeshComponent(this);
	my_meshc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
	//my_meshc->SetTextureIndex(1);
	setActive(false);
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(25, 25, 25);
}

CheckPoint::~CheckPoint() {
	// go to next level if there is one set
	if (level_string != "")
		mGame->setNextLevel(level_string);
}

void CheckPoint::setActive(bool b) {
	active = b;
	my_meshc->SetTextureIndex(!b);
}
