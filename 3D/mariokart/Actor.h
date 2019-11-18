#pragma once
#include <vector>
#include <SDL\SDL_stdinc.h>
#include "Math.h"

// enum class for actor class
enum class ActorState {
	Active,
	Paused,
	Destroy
};

class Actor {
public:
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	Vector3 GetForward() const;
	const Matrix4& GetWorldTransform() const { return m_world_transform; }
	
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class Game* GetGame() { return mGame; }
	
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);

	// removes component from actor; called in component destructor
	void RemoveComponent(class Component* c);
	
	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const {
		for (auto c : mComponents) {
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr) {
				return t;
			}
		}
		
		return nullptr;
	}
protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);
	
	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Matrix4 m_world_transform;
	void setWorldTransform();

	Vector3 mPosition;
	float mScale;
	float mRotation; // in radians
	
	// Components
	std::vector<class Component*> mComponents;
};
