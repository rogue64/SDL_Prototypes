#pragma once
#include "Actor.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "PlayerMovComp.h"

namespace plyr_const {
	static const int SIZE = 32;
};

class Player : public Actor {
protected:
	SpriteComponent* my_spr = nullptr;
	CollisionComponent* my_cc = nullptr;
	PlayerMovComp* my_mc = nullptr;

public:
	Player(Game* owner, Vector2 pos);
	~Player();
};

