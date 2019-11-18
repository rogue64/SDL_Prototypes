#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMovComp.h"

class PlayerMovComp;
class CameraComponent;

class Player : public Actor {
public:
	Player(Game* owner);
	~Player();
protected:
	MeshComponent* my_meshc;
	PlayerMovComp* my_pmc;
	bool space_pressed = false;
	CameraComponent* my_camc;

	void OnProcessInput(const Uint8* keyState) override;
};

