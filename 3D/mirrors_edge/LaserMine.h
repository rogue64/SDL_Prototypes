#pragma once
#include "Actor.h"
class LaserMine : public Actor {
public:
	LaserMine(class Game* owner);
	~LaserMine();
protected:
	class MeshComponent* my_meshc = nullptr;
	class LaserComponent* my_laser = nullptr;
};

