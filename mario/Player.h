#pragma once
#include "constants.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "PlayerMovComp.h"

class Player : public Actor {
protected:
	// important components
	SpriteComponent* my_spr = nullptr;
	CollisionComponent* my_cc = nullptr;
	PlayerMovComp* my_mc = nullptr;

public:
	Player(Game* owner, Vector2 pos);
	~Player();

	// returns pointer to collision component
	const CollisionComponent* getCollsion() const { return my_cc; }
};

