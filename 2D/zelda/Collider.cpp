#include "Collider.h"
#include "CollisionComponent.h"
#include "Game.h"

Collider::Collider(Game* owner, int x, int y, int w, int h) : Actor(owner) {
	// set collider size and width
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(w, h);
	mPosition = Vector2((w / 2) + x, (h / 2) + y);
	// add collider to game's vector
	owner->addCollider(this);
}

Collider::~Collider() { mGame->removeCollider(this); }
