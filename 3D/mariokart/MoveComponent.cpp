#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	// update angle
	mOwner->SetRotation(mOwner->GetRotation() + (deltaTime * mAngularSpeed));
	// update position
	mOwner->SetPosition((mOwner->GetForward() * deltaTime * mForwardSpeed) + mOwner->GetPosition());
}
