#pragma once
#include "MoveComponent.h"
#include "Player.h"

class Player;

class PlayerMovComp : public MoveComponent {
public:
	static enum class MoveState {
		on_ground,
		falling,
		jump,
		wall_climbing,
		wall_running,
	};
	PlayerMovComp(Player* owner);
	~PlayerMovComp();
	inline void setState(const MoveState& state) { my_mstate = state; }
	inline const MoveState getState() const { return my_mstate; }

protected:
	// z-speed which is used for jump/fall velocity
	//float z_speed = 0.f;
	MoveState my_mstate = MoveState::falling;
	Vector3 my_vel = Vector3::Zero;
	Vector3 my_accl = Vector3::Zero;
	Vector3 my_forc = Vector3::Zero; // pending forces
	// player's mass
	float my_mass = 1.f;
	class CameraComponent* owners_cam_comp = nullptr;
	// bool for checking front-edge jump click
	bool jump_pressed = false;
	// timer for how long we can climb
	float my_climb_timer = 0.0f;
	// timer for how long to wall-run
	float my_run_timer = 0.0f;
	class CameraComponent* owner_cam = nullptr;


	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void setPitchSpeed(const float& speed);
	// state-specific update 
	void updateFalling(const float& deltaTime);
	void updateJump(const float& deltaTime);
	void updateOnGround(const float& deltaTime);
	void updateWallClimb(const float& deltaTime);
	void updateWallRun(const float& deltaTime);
	// returns if we can wall climb based on CollSide given from oposing obj
	bool canWallClimb(CollSide side);
	// checks if the player can wall-run with the given Collside of opposing obj
	bool canWallRun(CollSide side);
	// fix position based on collision with given block
	CollSide fixCollision(CollisionComponent* self, CollisionComponent* other);
	// update position based on forces
	void updatePhysics(const float& deltaTime);
	// adds force to pending force
	inline void applyForce(const Vector3& force) { my_forc += force; }
	// clamps and decellerates velocity on x-y plane
	void fixXYVel();
};

