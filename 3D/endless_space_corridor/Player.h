#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMovComp.h"

class PlayerMovComp;

class Player : public Actor {
public:
	Player(Game* owner);
	~Player();
	CollisionComponent* getCollisionComp() const { return my_cc; }
protected:
	MeshComponent* my_meshc;
	CollisionComponent* my_cc;
	PlayerMovComp* my_pmc;
	bool space_pressed = false;

	void OnProcessInput(const Uint8* keyState) override;
};

