#pragma once
#include "Actor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Math.h"

class Laser : public Actor {
public:
	Laser(Game* owner, const Vector2& pos, const float& rot);
	~Laser();
protected:
	virtual void OnUpdate(float delta_time) override;
	// how long the laser has been "alive" (in seconds)
	float lifetime = 0;
};

