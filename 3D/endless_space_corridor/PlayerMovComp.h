#pragma once
#include "MoveComponent.h"
#include "Player.h"

class Player;

class PlayerMovComp : public MoveComponent {
public:
	PlayerMovComp(Player* owner);
	~PlayerMovComp();
protected:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

	int horz_input = 0;
	int vert_input = 0;
	float speed_timer = 0;
	float speed_mult = 1;
	// channel index for the player engine and dying sound
	int engine_channel;
};

