#pragma once
#include "Actor.h"
class Block : public Actor {
public:
	Block(class Game* owner, float x, float y, float z, bool explosive);
	virtual ~Block();
	const bool& isExplosive() const { return my_isExplosive; }
	const class CollisionComponent* getCollisionComp() const { return my_cc; }
	void explode();
protected:
	class CollisionComponent* my_cc = nullptr;
	class MeshComponent* my_meshc = nullptr;
	bool my_isExplosive = false;

	void OnUpdate(float deltaTime) override;
};

