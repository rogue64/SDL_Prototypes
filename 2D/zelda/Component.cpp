#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	,mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component() {
	// remove self from component list
	//mOwner->RemoveComponent(this); // broke it because of sprite component
}

void Component::Update(float deltaTime)
{
}

void Component::ProcessInput(const Uint8 *keyState)
{
}
