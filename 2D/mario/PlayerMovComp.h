#pragma once
#include "MoveComponent.h"
#include "Actor.h"

// collision component specifically for the player; 
// only Player class can create it else we have undefined collision behavior

class PlayerMovComp : public MoveComponent {
protected:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	// velocities for both axis
	float my_xSpeed = 0;
	float my_ySpeed = 0;
	// bools used for jumping input
	bool jump_pressed = false; // to tell us if jump pressed last frame
	bool in_air = false;

public:
	PlayerMovComp(Actor* owner);
	~PlayerMovComp();
};

