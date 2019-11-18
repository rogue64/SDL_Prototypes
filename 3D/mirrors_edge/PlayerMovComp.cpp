#include "PlayerMovComp.h"
#include "constants.h"
#include "SDL\SDL_scancode.h"
#include "Math.h"
#include "Game.h"
#include "Renderer.h"
#include "CameraComponent.h"

PlayerMovComp::PlayerMovComp(Player* owner) : MoveComponent((Actor*) owner) {
	owner_cam = mOwner->GetComponent<CameraComponent>();
}

PlayerMovComp::~PlayerMovComp() { }

void PlayerMovComp::Update(float deltaTime) {
	// call update function based on move state
	switch (my_mstate) {
	case(MoveState::falling):		updateFalling(deltaTime); break;
	case(MoveState::jump):			updateJump(deltaTime); break;
	case(MoveState::on_ground):		updateOnGround(deltaTime); break;
	case(MoveState::wall_climbing): updateWallClimb(deltaTime); break;
	case(MoveState::wall_running):	updateWallRun(deltaTime); break;
	default:						MoveComponent::Update(deltaTime); // by defualt, update move component; maybe not needed?
	}
	// check position to possibly respawn
	if (mOwner->GetPosition().z < -750) {
		my_vel = Vector3::Zero;
		my_forc = Vector3::Zero;
		setState(MoveState::falling);
		mOwner->SetRotation(0);
		mOwner->SetPosition(((Player*)mOwner)->getRespawnPosition());
	}
}

void PlayerMovComp::ProcessInput(const Uint8* keyState) {
	// get keyboard input to run and strafe
	Vector3 run_force = mOwner->GetForward();
	run_force *= plyr_c::MOVE_FORCE_MAG * (keyState[SDL_SCANCODE_W] - keyState[SDL_SCANCODE_S]);
	Vector3 strafe_force = mOwner->GetRight();
	strafe_force *= plyr_c::MOVE_FORCE_MAG * (keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A]);
	applyForce(run_force + strafe_force);
	// check if jump button pressed and is front-edge and on ground; if so, jump
	if (!jump_pressed && keyState[SDL_SCANCODE_SPACE] && my_mstate == MoveState::on_ground) {
		applyForce(plyr_c::JUMP_FORCE);
		setState(MoveState::jump);
	}
	jump_pressed = keyState[SDL_SCANCODE_SPACE];
	// get mouse input to rotate/turn
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	float hor_rotate_spd = ((float)x / MAX_MOUSE_MOV_X) * plyr_c::RAD_PER_SEC_TURNING;
	SetAngularSpeed(hor_rotate_spd);
	float rotate_pitch_spd = ((float)y / MAX_MOUSE_MOV_Y) * plyr_c::RAD_PER_SEC_TURNING;
	setPitchSpeed(rotate_pitch_spd);
}

void PlayerMovComp::setPitchSpeed(const float& speed) {
	if (!owners_cam_comp)
		owners_cam_comp = mOwner->GetComponent<CameraComponent>();
	if (!owners_cam_comp)
		return;
	owners_cam_comp->setPitchSpeed(speed);
}

void PlayerMovComp::updateFalling(const float& deltaTime) {
	// apply gravity
	applyForce(W_GRAV);
	updatePhysics(deltaTime);
	// check collisions at new position
	std::vector<Actor*> blocks = mOwner->GetGame()->getBlocks();
	for (int i = 0; i < blocks.size(); i++) {
		CollSide curr_side = fixCollision(mOwner->GetComponent<CollisionComponent>(), blocks[i]->GetComponent<CollisionComponent>());
		// if you land on top of block, then you are on ground and no longer falling
		if (curr_side == CollSide::Top) {
 			my_vel.z = 0;
			this->setState(MoveState::on_ground);
		}
	}
}

void PlayerMovComp::updateJump(const float& deltaTime) {
	// apply gravity
	applyForce(W_GRAV);
	updatePhysics(deltaTime);
	// check collisions to see if block is hit when moving up
	std::vector<Actor*> blocks = mOwner->GetGame()->getBlocks();
	for (int i = 0; i < blocks.size(); i++) {
		CollSide curr_side = fixCollision(mOwner->GetComponent<CollisionComponent>(), blocks[i]->GetComponent<CollisionComponent>());
		// check if player can wall climb
		if (canWallClimb(curr_side)) {
			my_climb_timer = 0.0f;
			setState(MoveState::wall_climbing);
			return;
		}
		// check if player can wall run
		if (canWallRun(curr_side)) {
			my_run_timer = 0.0f;
			setState(MoveState::wall_running);
			return;
		}
		// if block hit then all upward velocity stops
		if (curr_side == CollSide::Bottom)
		//if (curr_side != CollSide::None)
			my_vel.z = 0;
	}
	// if there is no verticle speed or negative verticle speed, start falling
	if (my_vel.z <= 0)
		setState(MoveState::falling);
}

void PlayerMovComp::updateOnGround(const float& deltaTime) {
	updatePhysics(deltaTime);
	// after moving, apply collisions
	std::vector<Actor*> blocks = mOwner->GetGame()->getBlocks();
	bool on_top = false; // bool to track if on top of block
	for (int i = 0; i < blocks.size(); i++) {
		CollSide curr_side = fixCollision(mOwner->GetComponent<CollisionComponent>(), blocks[i]->GetComponent<CollisionComponent>());
		// check if player can wall climb
		if (canWallClimb(curr_side)) {
			my_climb_timer = 0.0f;
			setState(MoveState::wall_climbing);
			return; // stop checking blocks once you know you can wall climb; also because you should only change states once per loop, so this doesn't allow state to get overwritten
		}
		if (curr_side == CollSide::Top) {
			on_top = true;
			//break;
		}
	}
	// if not on top of any block, then fall
	if (!on_top)
		setState(MoveState::falling);
}

void PlayerMovComp::updateWallClimb(const float& deltaTime) {
	// apply gravity
	applyForce(W_GRAV);
	if (my_climb_timer < plyr_c::CLIMB_LIMIT) {
		my_climb_timer += SEC * deltaTime;
		// apply wall climb force
		applyForce(plyr_c::CLIMB_FORCE);
	}
	updatePhysics(deltaTime);
	// check collisions
	bool collided = false; // track if any collision made
	std::vector<Actor*> blocks = mOwner->GetGame()->getBlocks();
	for (int i = 0; i < blocks.size(); i++) {
		CollSide curr_side = fixCollision(mOwner->GetComponent<CollisionComponent>(), blocks[i]->GetComponent<CollisionComponent>());
		if (curr_side != CollSide::None)
			collided = true;
	}
	if (!collided || my_vel.z <= 0) {
		my_vel.z = 0;
		setState(MoveState::falling);
	}
}

bool PlayerMovComp::canWallClimb(CollSide side) {
	if (side == CollSide::Bottom || side == CollSide::Top || side == CollSide::None)
		return false;
	Vector3 normal = Vector3::Zero;
	if (side == CollSide::SideXIn)
		normal = Vector3::UnitX;
	if (side == CollSide::SideXOut)
		normal = -1 * Vector3::UnitX;
	if (side == CollSide::SideYLeft)
		normal = -1 * Vector3::UnitY;
	if (side == CollSide::SideYRight)
		normal = Vector3::UnitY;
	// return if the normal is in opposite direction from player forward ( with leeway)
	return Vector3::Dot(normal, mOwner->GetForward()) < -1 + wall_climb_leeway;
}

void PlayerMovComp::updateWallRun(const float& deltaTime) {
	// apply gravity
	applyForce(W_GRAV);
	if (my_run_timer < plyr_c::CLIMB_LIMIT) {
		my_run_timer += SEC * deltaTime;
		// apply wall-run force 
		applyForce(plyr_c::W_RUN_FORCE);
	}
	updatePhysics(deltaTime);
	// check collisions
	bool collided = false; // track if any collision made
	std::vector<Actor*> blocks = mOwner->GetGame()->getBlocks();
	for (int i = 0; i < blocks.size(); i++) {
		CollSide curr_side = fixCollision(mOwner->GetComponent<CollisionComponent>(), blocks[i]->GetComponent<CollisionComponent>());
		if (curr_side != CollSide::None)
			collided = true;
		// change the camera-up to proper orienation
		if (curr_side == CollSide::SideXIn)
			owner_cam->setUpTarg(Vector3::Normalize(Vector3::UnitZ + Vector3::UnitX));
		else if (curr_side == CollSide::SideXOut)
			owner_cam->setUpTarg(Vector3::Normalize(Vector3::UnitZ - Vector3::UnitX));
		else if (curr_side == CollSide::SideYLeft)
			owner_cam->setUpTarg(Vector3::Normalize(Vector3::UnitZ - Vector3::UnitY));
		else if (curr_side == CollSide::SideYRight)
			owner_cam->setUpTarg(Vector3::Normalize(Vector3::UnitZ + Vector3::UnitY));
	}
	// not wall running anymore
	if (!collided || my_vel.z <= 0) {
		// set camera-up to normal orientation
		owner_cam->setUpTarg(Vector3::UnitZ);
		// fall
		setState(MoveState::falling);
		return;
	}
	
}

bool PlayerMovComp::canWallRun(CollSide side) {
	if (side == CollSide::Bottom || side == CollSide::Top || side == CollSide::None 
		|| my_vel.LengthSq() <= WALL_RUN_SPD_SQ || Vector3::Dot(Vector3::Normalize(my_vel), mOwner->GetForward()) < 0) 
		return false;
	Vector3 normal = Vector3::Zero;
	if (side == CollSide::SideXIn)
		normal = Vector3::UnitX;
	if (side == CollSide::SideXOut)
		normal = -1 * Vector3::UnitX;
	if (side == CollSide::SideYLeft)
		normal = -1 * Vector3::UnitY;
	if (side == CollSide::SideYRight)
		normal = Vector3::UnitY;
	float temp = Vector3::Dot(normal, mOwner->GetForward());
	// return if the normal is in perpindicular direction from player forward (with leeway)
	return ((temp > 0 - wall_climb_leeway) && (temp < 0 + wall_climb_leeway));
}

CollSide PlayerMovComp::fixCollision(CollisionComponent* self, CollisionComponent* other) {
	Vector3 offset;
	// check min overlap
	CollSide curr_side = other->GetMinOverlap(self, offset);
	// if no collision, return none and do nothing
	if (curr_side == CollSide::None)
		return curr_side;
	Vector3 normal = Vector3::Zero;
	if (curr_side == CollSide::SideXIn)
		normal = Vector3::UnitX;
	if (curr_side == CollSide::SideXOut)
		normal = -1 * Vector3::UnitX;
	if (curr_side == CollSide::SideYLeft)
		normal = -1 * Vector3::UnitY;
	if (curr_side == CollSide::SideYRight)
		normal = Vector3::UnitY;
	applyForce(normal * NORM_F_MAG);
	// else change position due to offset and return side
	mOwner->SetPosition(mOwner->GetPosition() + offset);
	return curr_side;
}

void PlayerMovComp::updatePhysics(const float& deltaTime) {
	// interpolate pending forces, acceleration, and velocity to move position
	my_accl = my_forc * (1 / my_mass);
	my_vel += my_accl * deltaTime;
	// fix xy speed
	fixXYVel();
	mOwner->SetPosition(mOwner->GetPosition() + (my_vel * deltaTime));
	// update angle based on input
	mOwner->SetRotation(mOwner->GetRotation() + (deltaTime * mAngularSpeed));
	// clear pending forces
	my_forc = Vector3::Zero;
}

void PlayerMovComp::fixXYVel() {
	// get velocity of just xy-plane
	Vector2 xy_vel = Vector2(my_vel.x, my_vel.y);
	// cap move speed
	if (xy_vel.LengthSq() > plyr_c::MAX_MOV_SPD_SQ) {
		xy_vel.Normalize();
		xy_vel *= plyr_c::MAX_MOV_SPD;
	}
	// when on ground, apply friction to deccelerate
	if (my_mstate != MoveState::on_ground && my_mstate != MoveState::wall_climbing) {
		my_vel.x = xy_vel.x;
		my_vel.y = xy_vel.y;
		return;
	}

	if (Math::NearZero(my_accl.x) || (xy_vel.x < 0 && my_accl.x > 0) || (xy_vel.x > 0 && my_accl.x < 0))
		xy_vel.x *= plyr_c::FRIC_COEF;
	if (Math::NearZero(my_accl.y) || (xy_vel.y < 0 && my_accl.y > 0) || (xy_vel.y > 0 && my_accl.y < 0))
		xy_vel.y *= plyr_c::FRIC_COEF;

	my_vel.x = xy_vel.x;
	my_vel.y = xy_vel.y;
}