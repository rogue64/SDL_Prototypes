#pragma once
#include "Actor.h"
class Block : public Actor {
public:
	Block(class Game* owner);
	~Block();
protected:
	class MeshComponent* my_meshc;
	class CollisionComponent* my_cc;
};

