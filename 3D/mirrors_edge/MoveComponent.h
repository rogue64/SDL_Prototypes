#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner);

	// Update the move component
	virtual void Update(float deltaTime) override;
	
	// Getters/setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
protected:
	// Angular speed (in radians/second)
	float mAngularSpeed = 0;
	// Forward speed (in pixels/second)
	float mForwardSpeed = 1;
};
