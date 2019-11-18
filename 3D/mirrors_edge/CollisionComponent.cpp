#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset) {
	CollSide out = CollSide::None;
	// if there is no intersection, then return data indicating so
	if (!Intersect(other)) {
		offset = Vector3::Zero;
		return out;
	}

	// store min and maxes from myself and other
	const Vector3 m_max = GetMax();
	const Vector3 m_min = GetMin();
	const Vector3 o_max = other->GetMax();
	const Vector3 o_min = other->GetMin();

	// calculate the distance of intersecting edges
	float diff_l, diff_r, diff_t, diff_b, diff_i, diff_o, min = 0;
	diff_l = Math::Abs(m_min.y - o_max.y); // left edge of me vs right edge of other
	diff_r = Math::Abs(m_max.y - o_min.y); // right edge of me vs left edge of other
	diff_t = Math::Abs(m_max.z - o_min.z); // top edge of me vs bottom edge of other
	diff_b = Math::Abs(m_min.z - o_max.z); // bottom edge of me vs top edge of other
	diff_i = Math::Abs(m_max.x - o_min.x); // furthest into screen of me vs furthest out of other
	diff_o = Math::Abs(m_min.x - o_max.x); // furthest out of screen me vs furthest in of other

	// whichever has the shortest difference, distance is the side we are colliding on
	// use this to populate the offset vector that other will have to use to move out of collision
	if (diff_t < diff_b) {
		min = diff_t;
		out = CollSide::Top;
		offset = Vector3(0, 0, min);
	}
	else {
		min = diff_b;
		out = CollSide::Bottom;
		offset = Vector3(0, 0, -min);
	}
	if (diff_l < min) {
		min = diff_l;
		out = CollSide::SideYLeft;
		offset = Vector3(0, -min, 0);
	}
	if (diff_r < min) {
		min = diff_r;
		out = CollSide::SideYRight;
		offset = Vector3(0,	min, 0);
	}
	if (diff_i < min) {
		min = diff_i;
		out = CollSide::SideXIn;
		offset = Vector3(min, 0, 0);
	}
	if (diff_o < min) {
		min = diff_o;
		out = CollSide::SideXOut;
		offset = Vector3(-min, 0, 0);
	}
	return out;
}
