#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f) { }

CollisionComponent::~CollisionComponent() { }

bool CollisionComponent::Intersect(const CollisionComponent* other) const {
	bool case1 = GetMin().x > other->GetMax().x;
	bool case2 = GetMin().y > other->GetMax().y;
	bool case3 = GetMax().x < other->GetMin().x;
	bool case4 = GetMax().y < other->GetMin().y;
	// if none of the cases are true then the boxes intersect
	return !(case1 || case2 || case3 || case4);
}

Vector2 CollisionComponent::GetMin() const {
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

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other,
										   Vector2& offset) const {
	CollSide out = CollSide::None;
	// if there is no intersection, then return data indicating so
	if (!Intersect(other)) { 
		offset = Vector2::Zero;
		return out;
	}

	// store min and maxes from myself and other
	const Vector2 m_max = GetMax();
	const Vector2 m_min = GetMin();
	const Vector2 o_max = other->GetMax();
	const Vector2 o_min = other->GetMin();

	// calculate the distance of intersecting edges
	float diff_l, diff_r, diff_t, diff_b, min = 0;
	diff_l = Math::Abs(m_min.x - o_max.x); // left edge of me vs right edge of other
	diff_r = Math::Abs(m_max.x - o_min.x); // right edge of me vs left edge of other
	diff_t = Math::Abs(m_min.y - o_max.y); // top edge of me vs bottom edge of other
	diff_b = Math::Abs(m_max.y - o_min.y); // bottom edge of me vs top edge of other

	// whichever has the shortest difference, distance is the side we are colliding on
	// use this to populate the offset vector that other will have to use to move out of collision
	if (diff_l < diff_r) {
		min = diff_l;
		out = CollSide::Left;
		offset = Vector2(-min, 0);
	}
	else {
		min = diff_r;
		out = CollSide::Right;
		offset = Vector2(min, 0);
	}
	if (diff_t < min) { 
		min = diff_t;
		out = CollSide::Top;
		offset = Vector2(0, -min);
	}
	if (diff_b < min) { 
		min = diff_b;
		out = CollSide::Bottom;
		offset = Vector2(0, min);
	}
	return out;
}
