#pragma once
#include "Game.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
#include "Actor.h"

class BallMovComp : public MoveComponent {
protected:
	// velocity of ball
	Vector2 my_vel = Vector2(250, -250);
	// cache sprite of owner
	SpriteComponent* owner_spr = nullptr;
	// cache the collision component of owner
	CollisionComponent* owner_cc = nullptr;
	// cache the collision component of paddle
	CollisionComponent* pddl_cc = nullptr;
public:
	BallMovComp(Actor* owner);
	~BallMovComp();
	void Update(float deltaTime) override;
};

