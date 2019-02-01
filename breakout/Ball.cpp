#include "Ball.h"

Ball::Ball(Game* owner) : Actor(owner) {
	// set ball's sprite with correct texture
	my_spr = new SpriteComponent(this);
	my_spr->SetTexture(owner->getTexture("Assets/Ball.png"));
	// after sprite set, use it's texture to get size for collision box
	my_cc = new CollisionComponent(this);
	my_cc->SetSize(my_spr->GetTexWidth(), my_spr->GetTexHeight());
	// ball's move component
	my_mc = new BallMovComp(this);
	// set it's initial position; bottom-center
	mPosition = Vector2(WINDOW_W / 2, WINDOW_H * .8);
}


Ball::~Ball() {

}
