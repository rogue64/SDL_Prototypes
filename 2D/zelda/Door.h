#pragma once
#include "constants.h"
#include "Actor.h"

class Door : public Actor {
public:
	Door(Game* owner, int x, int y, int w, int h, std::string dest,
		 std::string dir, std::string state, std::string room);
	~Door();
	void setState(DoorState state) { my_state = state; }
	const DoorState& getState() const { return my_state; }
	const Direction& getDirection() const { return my_dir; }
	const std::string& getDest() const { return destination; }
	const std::string& getRoom() const { return my_room; }
	const CollisionComponent* getCC() const { return my_cc; }
	// updates the animation/texture based on the state/dir of door
	void updateAnimation();
protected:
	// collision comp
	CollisionComponent* my_cc = nullptr;
	// animation component
	AnimatedSprite* my_spr = nullptr;
	// state of door (default)
	DoorState my_state = DoorState::Open;
	// direction door is facing/leading (default)
	Direction my_dir = Direction::Up;
	// room that the door leads to (default)
	std::string destination = "Room1"; 
	// room that the door is a part of
	std::string my_room = "Room1";
};

