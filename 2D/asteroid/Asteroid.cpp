#include "Asteroid.h"

Asteroid::Asteroid(Game* owner) : Actor(owner) {
	// move component initialized with speed of 150
	my_mc = new MoveComponent(this);
	my_mc->SetForwardSpeed(150);

	// randomize direction of asteroid
	mRotation = Random::GetFloatRange(0, 2 * Math::Pi);

	// randomize start position of asteroid
	mPosition = Random::GetVector(Vector2(0, 0), Vector2(WINDOW_W, WINDOW_H));

	// initialize sprite with right texture
	my_spr = new SpriteComponent(this);
	my_spr->SetTexture(owner->getTexture("Assets/Asteroid.png"));

	// add asteroid to game's asteroid vector
	owner->addAsteroid(this);
}

Asteroid::~Asteroid() {
	// remove asteroid from game's asteroid vector
	mGame->removeAsteroid(this);
}

void Asteroid::OnUpdate(float delta_time) {
	// wrap asteroid around screen
	if (mPosition.x < 0)
		mPosition.x = WINDOW_W - 1;
	if (mPosition.x >= WINDOW_W)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = WINDOW_H - 1;
	if (mPosition.y >= WINDOW_H)
		mPosition.y = 0;
}
