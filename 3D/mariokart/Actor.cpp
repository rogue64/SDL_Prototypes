#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f) {
	// add self to mGame's Actor* vector
	mGame->AddActor(this);
}

Actor::~Actor() {
	// remove self from mGame's Actor* vector
	mGame->RemoveActor(this);
	// destroy components and clear component vector
	for (size_t i = 0; i < mComponents.size(); i++)
		if (mComponents[i])
			delete mComponents[i];
	mComponents.clear();
}

void Actor::Update(float deltaTime) {
	// if object is active then update all it's components and self
	if (mState == ActorState::Active) {
		for (size_t i = 0; i < mComponents.size(); i++)
			mComponents[i]->Update(deltaTime);
		OnUpdate(deltaTime);
	}

	setWorldTransform();
}

void Actor::OnUpdate(float deltaTime) { }

void Actor::ProcessInput(const Uint8* keyState) {
	// if object is active then proccess inputs for all it's components and self
	if (mState == ActorState::Active) {
		for (size_t i = 0; i < mComponents.size(); i++)
			mComponents[i]->ProcessInput(keyState);
		this->OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState) { }

void Actor::AddComponent(Component* c) {
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

void Actor::RemoveComponent(class Component* c) { 
	std::vector<class Component*>::iterator i = mComponents.begin();
	for (; i != mComponents.end(); i++)
		if (*i == c)
			break;
	if (i != mComponents.end())
		mComponents.erase(i);
}

Vector3 Actor::GetForward() const {
	return Vector3(Math::Cos(mRotation), Math::Sin(mRotation), 0);
}

void Actor::setWorldTransform() {
	// do matrix calculation in the order of Senior T (S,R,T)

	// scale matrix
	Matrix4 scale = Matrix4::CreateScale(mScale);
	// rotation matrix
	Matrix4 rotationZ = Matrix4::CreateRotationZ(mRotation);
	// translate(position) matrix
	Matrix4 translate = Matrix4::CreateTranslation(mPosition);

	m_world_transform = scale * rotationZ;
	m_world_transform *= translate;
}
