#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f) {
	
}

CollisionComponent::~CollisionComponent() {
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other) {
	// TODO: Implement
	bool case1 = GetMin().x > other->GetMax().x;
	bool case2 = GetMin().y > other->GetMax().y;
	bool case3 = GetMax().x < other->GetMin().x;
	bool case4 = GetMax().y < other->GetMin().y;
	// if none of the cases are true then the boxes intersect
	return !(case1 || case2 || case3 || case4);
}

Vector2 CollisionComponent::GetMin() const	{
	// TODO: Implement
	float out_x = mOwner->GetPosition().x - ((mWidth * mOwner->GetScale()) / 2);
	float out_y = mOwner->GetPosition().y - ((mHeight * mOwner->GetScale()) / 2);
	return Vector2(out_x, out_y);
}

Vector2 CollisionComponent::GetMax() const	{
	// TODO: Implement
	float out_x = mOwner->GetPosition().x + ((mWidth * mOwner->GetScale()) / 2);
	float out_y = mOwner->GetPosition().y + ((mHeight * mOwner->GetScale()) / 2);
	return Vector2(out_x, out_y);
}

const Vector2& CollisionComponent::GetCenter() const {
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)	{
	offset = Vector2::Zero;
	// TODO: Implement
	return CollSide::None;
}
