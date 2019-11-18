#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(my_actorstate::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
	// add self to mGame's Actor* vector
	mGame->addActor(this);
}

Actor::~Actor() {
	// remove self from mGame's Actor* vector
	mGame->removeActor(this);
	// destroy components and clear component vector
	for (size_t i = 0; i < mComponents.size(); i++)
		if (mComponents[i])
			delete mComponents[i];
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	// if object is active then update all it's components and self
	if (mState == my_actorstate::Active) {
		for (size_t i = 0; i < mComponents.size(); i++)
			mComponents[i]->Update(deltaTime);
		OnUpdate(deltaTime);
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	// if object is active then proccess inputs for all it's components and self
	if (mState == my_actorstate::Active) {
		for (size_t i = 0; i < mComponents.size(); i++)
			mComponents[i]->ProcessInput(keyState);
		this->OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

Vector2 Actor::GetForward() const {
	return Vector2(Math::Cos(mRotation), -1 * Math::Sin(mRotation));
}
