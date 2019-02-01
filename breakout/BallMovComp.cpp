#include "BallMovComp.h"

BallMovComp::BallMovComp(Actor* owner) : MoveComponent(owner) { }

BallMovComp::~BallMovComp() { }

void BallMovComp::Update(float deltaTime) {
	// update position due to velocity
	mOwner->SetPosition(mOwner->GetPosition() + (my_vel * deltaTime));
	// check if hitting left or right wall
	if (!owner_spr)
		owner_spr = mOwner->GetComponent<SpriteComponent>();
	if (owner_spr) {
		float center_oset_x = owner_spr->GetTexWidth() / 2;
		float center_oset_y = owner_spr->GetTexHeight() / 2;
		// if right or left wall hit, flip x_vel and take care of collision
		if (mOwner->GetPosition().x + center_oset_x > WINDOW_W - WALL_OSET) {
			mOwner->SetPosition(Vector2(WINDOW_W - WALL_OSET - center_oset_x, mOwner->GetPosition().y));
			my_vel = Vector2(-my_vel.x, my_vel.y);
		}
		if (mOwner->GetPosition().x - center_oset_x < WALL_OSET) {
			mOwner->SetPosition(Vector2(WALL_OSET + center_oset_x, mOwner->GetPosition().y));
			my_vel = Vector2(-my_vel.x, my_vel.y);
		}
		// if top wall hit, flip y-vel and take care of collison
		if (mOwner->GetPosition().y - center_oset_y < WALL_OSET) {
			mOwner->SetPosition(Vector2(mOwner->GetPosition().x, WALL_OSET + center_oset_y));
			my_vel = Vector2(my_vel.x, -my_vel.y);
		}
		// if ball goes off screen at bottom, place it back in it's original position and velocity
		if (mOwner->GetPosition().y + center_oset_y > WINDOW_H) {
			mOwner->SetPosition( Vector2(WINDOW_W / 2.0, WINDOW_H * .8) );
			my_vel = Vector2(250, -250);
		}
	}
	Paddle* game_paddle = mOwner->GetGame()->getPaddle();
	if (!owner_cc || !pddl_cc) {
		owner_cc = mOwner->GetComponent<CollisionComponent>();
		pddl_cc = game_paddle->GetComponent<CollisionComponent>();
	}
	if (owner_cc && pddl_cc && (pddl_cc->Intersect(owner_cc))) {
		/*Vector2 normal = Vector2(0, -1);
		if (mOwner->GetPosition() < game_paddle->GetPosition - */
		my_vel = Vector2(my_vel.x, -my_vel.y);
		mOwner->SetPosition( Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y - 
			(game_paddle->GetComponent<SpriteComponent>()->GetTexHeight() / 2) ));
	}
}
