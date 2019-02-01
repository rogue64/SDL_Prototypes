#pragma once
#include "MoveComponent.h"
#include "Actor.h"

class PlayerMovComp : public MoveComponent {
public:
	PlayerMovComp(Actor* owner);
	~PlayerMovComp();
};

