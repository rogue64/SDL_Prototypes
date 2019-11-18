#include "SecretBlock.h"

SecretBlock::SecretBlock(Game* owner, int x, int y, int w, int h, std::string dir,
						 std::string room) : Collider(owner, x, y, w, h) {
	// set start position and room
	start_pos.x = x;
	start_pos.y = y;
	my_room = room;
	
	// set direction
	if (dir == "Left")
		my_dir = Direction::Left;
	else if (dir == "Right")
		my_dir = Direction::Right;
	else if (dir == "Down")
		my_dir = Direction::Down;
	else
		my_dir = Direction::Up;
}

SecretBlock::~SecretBlock() { }

void SecretBlock::OnUpdate(float deltaTime) { 
	// do nothing if the room already unlocked
	if (unlocked)
		return;
	
	// check collision
}
