#pragma once
#include "MoveComponent.h"
#include "Actor.h"

class PlayerMovComp : public MoveComponent {
protected:
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

public:
	PlayerMovComp(Actor* owner);
	~PlayerMovComp();
};

