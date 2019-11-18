#pragma once
#include"constants.h"
#include "Actor.h"
class Secret : public Actor {
public:
	Secret(Game* owner, int x, int y, int w, int h, std::string dir, std::string room);
	~Secret();
	const Direction& getDir() const { return my_dir; }
	// moves block in direction; called when player collides on correct side
	void moveSecret(float deltaTime);
	const CollisionComponent* getCC() const { return my_cc; }
protected:
	CollisionComponent* my_cc = nullptr;
	SpriteComponent* my_spr = nullptr;
	Vector2 start_pos;
	std::string my_room;
	Direction my_dir;
	int max_dist = TILE_SIZE;
	bool unlocked = false;
};

