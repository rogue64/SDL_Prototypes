#pragma once
#include "Actor.h"
class Bullet : public Actor {
public:
	Bullet(class Game*, Vector3 pos, float speed);
	~Bullet();
protected:
	class MeshComponent* my_meshc = nullptr;
	class MoveComponent* my_mc = nullptr;
	class CollisionComponent* my_cc = nullptr;

	float death_timer = 0;
	
	void OnUpdate(float deltaTime) override;
};

