#pragma once
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "BallMovComp.h"
#include "CollisionComponent.h"

class BallMovComp;

class Ball : public Actor {
protected:
	SpriteComponent* my_spr = nullptr;
	BallMovComp* my_mc = nullptr;
	CollisionComponent* my_cc = nullptr;
public:
	Ball(Game* owner);
	~Ball();
};

