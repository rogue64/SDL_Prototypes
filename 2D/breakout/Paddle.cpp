#include "Paddle.h"

Paddle::Paddle(Game* owner) : Actor(owner) {
	// create important components
	my_spr = new SpriteComponent(this);
	my_spr->SetTexture(owner->getTexture("Assets/Paddle.png"));
	// after sprite set, use it's texture to get size for collision box
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(my_spr->GetTexWidth(), my_spr->GetTexHeight());
	my_mc = new MoveComponent(this);
	// set intitial poisition, always bottom center
	mPosition = Vector2(WINDOW_W / 2, WINDOW_H * .9);
}

Paddle::~Paddle() { }

void Paddle::OnProcessInput(const Uint8* keyState) {
	// move paddle at paddle speed, based on input
	my_mc->SetForwardSpeed(PADDL_SPD * (keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]));
}

void Paddle::OnUpdate(float deltaTime) {
	// distance from center to horizontal end of paddle
	float center_oset = my_spr->GetTexWidth() / 2;
	// check right wall
	if (mPosition.x + center_oset > WINDOW_W - WALL_OSET)
		mPosition.x = WINDOW_W - WALL_OSET - center_oset;
	if (mPosition.x - center_oset < WALL_OSET)
		mPosition.x = WALL_OSET + center_oset;
}