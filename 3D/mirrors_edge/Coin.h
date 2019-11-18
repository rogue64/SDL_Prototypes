#pragma once
#include "Actor.h"
class Coin : public Actor {
public:
	Coin(Game* owner);
	~Coin();
protected:
	class MeshComponent* my_meshc = nullptr;
	class CollisionComponent* my_cc = nullptr;
	void OnUpdate(float deltaTime) override;
};