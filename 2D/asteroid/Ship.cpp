#include "Ship.h"

Ship::Ship(Game* owner) : Actor(owner) {
	// store pointers for move and sprite components
	my_mc = new MoveComponent(this);
	my_spr = new SpriteComponent(this);
	// store textures that the sprite component will alternate between
	idle_img = owner->getTexture("Assets/Ship.png");
	move_img = owner->getTexture("Assets/ShipThrust.png");
	my_spr->SetTexture(idle_img);
};

void Ship::OnProcessInput(const Uint8* key_state) {
	// change sprite when speed is being applied through input
	if (key_state[SDL_SCANCODE_UP])
		my_spr->SetTexture(move_img);
	else
		my_spr->SetTexture(idle_img);

	// increase/decrease movement speed
	my_mc->SetForwardSpeed(my_mc->GetForwardSpeed() + (key_state[SDL_SCANCODE_UP] * ship_const::accl)
		+ (!key_state[SDL_SCANCODE_UP] * ship_const::dccl));
	// cap speed in either direction
	if (my_mc->GetForwardSpeed() > ship_const::pvel_cap)
		my_mc->SetForwardSpeed(ship_const::pvel_cap);
	if (my_mc->GetForwardSpeed() < ship_const::nvel_cap)
		my_mc->SetForwardSpeed(ship_const::nvel_cap);

	// change angular velocity depending on input
	my_mc->SetAngularSpeed(my_mc->GetAngularSpeed() + ((key_state[SDL_SCANCODE_LEFT] * ship_const::angle_accl)
		- (key_state[SDL_SCANCODE_RIGHT] * ship_const::angle_accl)));
	// cap angular acceleration
	if (my_mc->GetAngularSpeed() > ship_const::angle_accl_cap)
		my_mc->SetAngularSpeed(ship_const::angle_accl_cap);
	if (my_mc->GetAngularSpeed() < -1 * ship_const::angle_accl_cap)
		my_mc->SetAngularSpeed(-1 * ship_const::angle_accl_cap);
	// deccelerate angle velocity when no side input
	if ((key_state[SDL_SCANCODE_LEFT] - key_state[SDL_SCANCODE_RIGHT]) == 0) {
		my_mc->SetAngularSpeed(0);
		if (my_mc->GetAngularSpeed() > 0) {
			my_mc->SetAngularSpeed(my_mc->GetAngularSpeed() - ship_const::angle_dccl);
			if (my_mc->GetAngularSpeed() < 0)
				my_mc->SetAngularSpeed(0);
		}
		if (my_mc->GetAngularSpeed() < 0) {
			my_mc->SetAngularSpeed(my_mc->GetAngularSpeed() + ship_const::angle_dccl);
			if (my_mc->GetAngularSpeed() > 0)
				my_mc->SetAngularSpeed(0);
		}
	}

	// fire meh laser (if not on cooldown)
	if (key_state[SDL_SCANCODE_SPACE] && cooldown > ship_const::cooldown_limit) {
		new Laser(mGame, mPosition, mRotation);
		cooldown = 0;
	}
}

void Ship::OnUpdate(float delta_time) {
	// wrap ship around screen
	if (mPosition.x < 0)
		mPosition.x = WINDOW_W - 1;
	if (mPosition.x >= WINDOW_W)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = WINDOW_H - 1;
	if (mPosition.y >= WINDOW_H)
		mPosition.y = 0;

	// increase laser cooldown by delta_time
	cooldown += delta_time;
}
