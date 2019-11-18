#pragma once
#include "Actor.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Random.h"

class Asteroid : public Actor {
public:
	Asteroid(Game* owner);
	~Asteroid();
protected:
	virtual void OnUpdate(float delta_time) override;
	MoveComponent* my_mc = nullptr;
	SpriteComponent* my_spr = nullptr;
};

