#include "Laser.h"

Laser::Laser(Game* owner, const Vector2& pos, const float& rot) : Actor(owner) {
	// set position and rotation with given arguments
	mPosition = pos;
	mRotation = rot;

	// Instantiate/Initialize MoveComponent and SpriteComponent
	MoveComponent* my_mc = new MoveComponent(this);
	my_mc->SetForwardSpeed(400);
	SpriteComponent* my_spr = new SpriteComponent(this);
	my_spr->SetTexture(owner->getTexture("Assets/Laser.png"));
}

Laser::~Laser() {
}

void Laser::OnUpdate(float delta_time) {
	// if laser collides with an asteroid, destory both
	Asteroid* ast_collidee = mGame->asteroidCollision(this);
	if (ast_collidee) {
		ast_collidee->SetState(my_actorstate::Destroy);
		this->mState = my_actorstate::Destroy;
		return;
	}

	// destroy self when laser is alive for a second
	lifetime += delta_time;
	if (lifetime > 1)
		mState = my_actorstate::Destroy;
}
