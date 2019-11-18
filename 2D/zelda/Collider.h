#pragma once
#include "constants.h"
#include "Actor.h"

//class CollisionComponent;

class Collider : public Actor {
public:
	Collider(Game* owner, int x, int y, int w, int h);
	virtual ~Collider();
	const CollisionComponent* getCC() const { return my_cc; }
protected:
	CollisionComponent* my_cc;
};

