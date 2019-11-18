#pragma once
#include "constants.h"
#include "Collider.h"
class SecretBlock :public Collider {
public:
	SecretBlock(Game* owner, int x, int y, int w, int h, std::string dir, std::string room);
	~SecretBlock();
protected:
	void OnUpdate(float deltaTime) override;
	std::string my_room;
	Vector2 start_pos;
	int max_dist = TILE_SIZE;
	Direction my_dir;
	bool unlocked = false;
};

