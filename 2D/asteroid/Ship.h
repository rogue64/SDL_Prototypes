#pragma once
#include "Actor.h"
#include "Game.h"
#include "Laser.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

namespace ship_const {
	static const int angle_accl = 5;
	static const int angle_dccl = 2;
	static const int angle_accl_cap = 8;
	// accerlaration, decelleration, and their caps
	static int const accl = 30;
	static int const pvel_cap = 300; // pos-velocity cap
	static int const dccl = -8;
	static int const nvel_cap = 0; // neg-velocity cap
	// cooldown for laser (in seconds)
	static float const cooldown_limit = 1.;
};

class Ship : public Actor {
public:
	Ship(Game* owner);
	~Ship() {};
	
protected:
	virtual void OnProcessInput(const Uint8* keyState) override;
	virtual void OnUpdate(float delta_time) override;
	// caching MoveComponent pointer
	MoveComponent* my_mc = nullptr;
	// caching SpriteComponent pointer
	SpriteComponent* my_spr = nullptr;
	// caching both textures
	SDL_Texture* idle_img = nullptr;
	SDL_Texture* move_img = nullptr;
	// keep track of laser cooldown
	float cooldown = ship_const::cooldown_limit;
};

