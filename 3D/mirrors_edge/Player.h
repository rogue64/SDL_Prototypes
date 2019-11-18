#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMovComp.h"
#include "Math.h"

class PlayerMovComp;
class CameraComponent;
class CollisionComponent;

class Player : public Actor {
public:
	Player(Game* owner);
	~Player();
	inline void setRespawnPosition(const Vector3& pos) { my_spawn_pos = pos; }
	inline const Vector3& getRespawnPosition() const { return my_spawn_pos; }

protected:
	PlayerMovComp* my_pmc;
	CameraComponent* my_camc;
	CollisionComponent* my_cc;
	Vector3 my_spawn_pos = Vector3::Zero;
	class HUD* my_hud;

	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
};