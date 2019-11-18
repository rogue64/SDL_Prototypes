#pragma once
#include "constants.h"
#include "Actor.h"

class Player : public Actor {
public:
	Player(Game* owner, Vector2 pos);
	~Player();
	// returns pointer to collision component
	const CollisionComponent* getCollsion() const { return my_cc; }
	// returns pointer to collision component
	const AnimatedSprite* getAnimation() const { return my_spr; }
	const Direction& getDirection() const { return my_dir; }
	const std::string& getRoom() const { return my_room; }
	void setRoom(std::string room) { my_room = room; }
protected:
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
	// important components
	AnimatedSprite* my_spr = nullptr;
	CollisionComponent* my_cc = nullptr;
	PlayerMovComp* my_mc = nullptr;
	// input bools for animations
	bool leftPressed = false, rightPressed = false, upPressed = false, downPressed = false;
	Direction my_dir = Direction::Down;
	std::string my_room = "Room1";
};

