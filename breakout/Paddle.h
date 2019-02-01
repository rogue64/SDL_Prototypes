#pragma once
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

class Paddle : public Actor {
protected:
	// pointers to important components
	SpriteComponent* my_spr = nullptr;
	MoveComponent* my_mc = nullptr;
	CollisionComponent* my_cc = nullptr;
	// move paddle based on input
	void OnProcessInput(const Uint8* keyState) override;
	// make sure paddle doesnt move past wall
	void OnUpdate(float deltaTime) override;
public:
	Paddle(Game* owner);
	~Paddle();
};

