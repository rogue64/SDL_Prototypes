#pragma once
#include "Actor.h"
class CheckPointArrow : public Actor {
public:
	CheckPointArrow(Game* owner);
	~CheckPointArrow();

private:
	class MeshComponent* my_meshc = nullptr;

	void OnUpdate(float deltaTime) override;
};

