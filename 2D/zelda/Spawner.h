#pragma once
#include "constants.h"
#include "Actor.h"

class Spawner : public Actor {
public:
	Spawner(Game* owner, Vector2 pos);
	~Spawner();
protected:
	//void OnUpdate(float deltaTime) override;
};

