#pragma once
#include "constants.h"
#include "MoveComponent.h"
#include "Game.h"


// move component specifically for the player; 
// only Player class can create it else we have undefined collision behavior

class PlayerMovComp : public MoveComponent {
public:
	PlayerMovComp(Actor* owner);
	~PlayerMovComp();
protected:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	// velocities for both axis
	Vector2 my_vel = Vector2::Zero;
	// bools for if horizontal or verticle input axis pressed
	bool hori_pressed = false;
	bool vert_pressed = false;
};

